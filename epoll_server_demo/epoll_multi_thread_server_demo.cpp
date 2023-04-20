#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <cstring>


const int PORT = 8888;
const int MAX_EVENTS = 10;
const int THREAD_POOL_SIZE = 4;

class ThreadPool
{
public:
    ThreadPool(size_t num_threads)
    {
        for (size_t i = 0; i < num_threads; ++i)
        {
            threads.emplace_back([this]()
                                 {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this]() { return !tasks.empty() || stop; });
                        if (stop) break;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                } });
        }
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
        for (auto &thread : threads)
        {
            thread.join();
        }
    }

    void enqueue(std::function<void()> task)
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.push(task);
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop = false;
};

void handle_client_data(int client_fd)
{
    char buffer[1024];
    ssize_t n = recv(client_fd, buffer, sizeof(buffer), 0);
    if (n <= 0)
    {
        if (n == 0)
        {
            std::cout << "Client closed connection" << std::endl;
        }
        else
        {
            perror("recv");
        }
        close(client_fd);
    }
    else
    {
        if (send(client_fd, buffer, n, 0) < 0)
        {
            perror("send");
            close(client_fd);
        }
    }
}

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        return EXIT_FAILURE;
    }

    sockaddr_in listen_addr;
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(PORT);

    if (bind(listen_fd, (sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
    {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(listen_fd, 5) < 0)
    {
        perror("listen");
        return EXIT_FAILURE;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) < 0)
    {
        perror("epoll_ctl EPOLL_CTL_ADD");
        return EXIT_FAILURE;
    }

    ThreadPool thread_pool(THREAD_POOL_SIZE);

    while (true)
    {
        epoll_event events[MAX_EVENTS];
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events < 0)
        {
            perror("epoll_wait");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < num_events; i++)
        {
            if (events[i].data.fd == listen_fd)
            {
                sockaddr_in client_addr;
                socklen_t client_addr_len;
                int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &client_addr_len);
                if (client_fd < 0)
                {
                    perror("accept");
                    continue;
                }

                event.data.fd = client_fd;
                event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0)
                {
                    perror("epoll_ctl EPOLL_CTL_ADD");
                    return EXIT_FAILURE;
                }
            }
            else
            {
                int client_fd = events[i].data.fd;
                thread_pool.enqueue([client_fd]()
                                    { handle_client_data(client_fd); });
            }
        }
    }

    close(listen_fd);
    close(epoll_fd);

    return 0;
}
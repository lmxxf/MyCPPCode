#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

/*
这段代码是一个 C 语言编写的简单的基于 epoll 的 TCP 服务器。它可以处理多个客户端连接，并在接收到客户端的数据后，将数据回传给客户端。下面是对代码的解释：
1. 引入所需的头文件。
2. 定义一个宏 MAX_EVENTS，表示 epoll_wait 函数一次返回的最大事件数。
3. 定义 main 函数，接收命令行参数。
在 main 函数内部，进行以下操作：
4. 创建一个 TCP 监听套接字，并检查是否创建成功。
5. 初始化 sockaddr_in 结构体变量 listen_addr，并将其设置为 IPv4（AF_INET），使用任意 IP 地址（INADDR_ANY），并将端口设置为 8888。
6. 使用 bind() 函数将套接字与服务器地址和端口绑定，并检查是否绑定成功。
7. 使用 listen() 函数监听端口，并设置允许的最大挂起连接数为 5。检查是否监听成功。
8. 创建一个 epoll 实例，并检查是否创建成功。
9. 将监听套接字添加到 epoll 实例中，并设置其关注的事件为 EPOLLIN（表示可读）和 EPOLLET（表示边缘触发模式）。
在一个无限循环中，服务器执行以下操作：
10. 使用 epoll_wait() 函数等待事件发生，并将发生的事件存储在 events 数组中。检查是否有错误发生。
11. 遍历发生的事件，并进行处理。如果事件对应的文件描述符（fd）是监听套接字，表示有新的客户端连接，执行以下操作： a. 使用 accept() 函数接受客户端连接，并获取客户端套接字。 b. 将客户端套接字添加到 epoll 实例中，并设置其关注的事件为 EPOLLIN 和 EPOLLET。 如果事件对应的文件描述符不是监听套接字，表示有客户端数据到达，执行以下操作： a. 使用 recv() 函数接收客户端数据。 b. 如果接收到的数据长度为 0 或负数，表示客户端关闭了连接或发生了错误。关闭对应的客户端套接字。 c. 如果接收到的数据长度为正数，表示接收到客户端数据。使用 send() 函数将接收到的数据回传给客户端。如果发送失败，关闭对应的客户端套接字。
在无限循环外：
12. 关闭监听套接字和 epoll 实例。
13. 返回 0，表示程序正常结束。
这个基于 epoll 的服务器可以同时处理多个客户端连接，并在接收到客户端的数据后将数据回传给客户端，具有较高的性能。

*/
#define MAX_EVENTS 10

int main(int argc, char *argv[])
{
    int listen_fd, client_fd, epoll_fd, num_events, i;
    struct sockaddr_in listen_addr, client_addr;
    socklen_t client_addr_len;
    struct epoll_event events[MAX_EVENTS], event;

    // Create a TCP socket for listening
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to a specific address and port
    memset(&listen_addr, 0, sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_addr.sin_port = htons(8888);
    if (bind(listen_fd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(listen_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Create an epoll instance
    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // Add the listening socket to the epoll instance
    event.data.fd = listen_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) < 0)
    {
        perror("epoll_ctl EPOLL_CTL_ADD");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        // Wait for events
        num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events < 0)
        {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // Handle events
        for (i = 0; i < num_events; i++)
        {
            if (events[i].data.fd == listen_fd)
            {
                // Handle a new client connection
                client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (client_fd < 0)
                {
                    perror("accept");
                    continue;
                }

                // Add the client socket to the epoll instance
                event.data.fd = client_fd;
                event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) < 0)
                {
                    perror("epoll_ctl EPOLL_CTL_ADD");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                // Handle client data
                char buffer[1024];
                ssize_t n = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (n <= 0)
                {
                    // Client closed connection or error occurred
                    if (n == 0)
                    {
                        printf("Client closed connection\n");
                    }
                    else
                    {
                        perror("recv");
                    }
                    close(events[i].data.fd);
                }
                else
                {
                    // Echo client message back to client
                    if (send(events[i].data.fd, buffer, n, 0) < 0)
                    {
                        perror("send");
                        close(events[i].data.fd);
                    }
                }
            }
        }
    }
    // Close the listening socket and epoll instance
    close(listen_fd);
    close(epoll_fd);

    return 0;
}
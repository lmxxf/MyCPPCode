#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

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
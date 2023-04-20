#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
这段代码是一个简单的 C++ 编写的 TCP 服务器，使用了套接字（socket）进行通信。以下是对代码的解释：
1. 引入所需的头文件。
2. 定义一个常量 PORT，表示服务器要监听的端口，这里设置为 8080。
3. 定义 main 函数。
在 main 函数内部，进行以下操作：
4. 创建一个 TCP socket，使用 socket(AF_INET, SOCK_STREAM, 0)。如果失败，返回错误信息并退出程序。
5. 设置套接字选项为地址重用（SO_REUSEADDR），防止地址占用问题。
6. 定义一个 sockaddr_in 结构体变量，用于存储服务器地址和端口信息。将其设置为 IPv4（AF_INET），使用任意 IP 地址（INADDR_ANY），并将端口设置为预定义的 PORT。
7. 使用 bind() 函数将套接字与服务器地址和端口绑定。如果失败，返回错误信息并退出程序。
8. 使用 listen() 函数监听端口。这里的参数 5 表示允许的最大挂起连接数。如果失败，返回错误信息并退出程序。
9. 输出服务器正在监听的端口信息。
在一个无限循环中，服务器执行以下操作：
10. 使用 accept() 函数接受客户端连接。如果失败，返回错误信息并继续监听。
11. 输出客户端已连接的信息。
12. 定义一个欢迎消息，并通过 send() 函数发送给客户端。如果发送失败，返回错误信息。
13. 使用 close() 函数关闭客户端连接。
在无限循环外：
14. 使用 close() 函数关闭服务器套接字。
15. 返回 0，表示程序正常结束。
这个简单的服务器只能处理一个客户端连接请求，发送欢迎消息后就关闭连接。要实现并发处理多个客户端，可以使用多线程或异步 I/O 方式。
*/
const int PORT = 8080;

int main()
{
    // 创建 socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    // 设置地址重用选项，避免端口占用问题
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Failed to set socket options." << std::endl;
        return -1;
    }

    // 设置地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // 绑定地址和端口
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to bind address." << std::endl;
        return -1;
    }

    // 监听端口
    if (listen(server_fd, 5) == -1) {
        std::cerr << "Failed to listen port." << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true) {
        // 接受客户端连接
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            std::cerr << "Failed to accept client." << std::endl;
            continue;
        }

        std::cout << "Client connected." << std::endl;

        // 发送欢迎消息
        const char* msg = "Welcome to the server!\n";
        if (send(client_fd, msg, strlen(msg), 0) == -1) {
            std::cerr << "Failed to send message." << std::endl;
        }

        // 关闭客户端连接
        close(client_fd);
    }

    // 关闭服务器 socket
    close(server_fd);

    return 0;
}

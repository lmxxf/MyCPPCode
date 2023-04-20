#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

int main()
{
    // 创建 socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return -1;
    }

    // 设置地址和端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    addr.sin_port = htons(PORT);

    // 连接服务器
    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to connect to server." << std::endl;
        return -1;
    }

    std::cout << "Connected to server." << std::endl;

    // 接收服务器端消息
    char buffer[1024] = {0};
    if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
        std::cerr << "Failed to receive message." << std::endl;
    } else {
        std::cout << "Message from server: " << buffer;
    }

    // 关闭客户端 socket
    close(client_fd);

    return 0;
}

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int PORT = 8888;

int main() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(client_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return EXIT_FAILURE;
    }

    std::cout << "Connected to server." << std::endl;

    const char *message = "Hello, server!";
    if (send(client_fd, message, strlen(message), 0) < 0) {
        perror("send");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    ssize_t received_bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (received_bytes < 0) {
        perror("recv");
        return EXIT_FAILURE;
    }

    buffer[received_bytes] = '\0';
    std::cout << "Received from server: " << buffer << std::endl;

    close(client_fd);
    return 0;
}

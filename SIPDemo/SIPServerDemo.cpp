#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// SIP 请求消息模板
const std::string SIP_REQUEST_TEMPLATE = "INVITE sip:%s SIP/2.0\r\n"
                                         "Via: SIP/2.0/TCP %s;branch=z9hG4bK%s\r\n"
                                         "From: <sip:caller@%s>;tag=%s\r\n"
                                         "To: <sip:callee@%s>\r\n"
                                         "Call-ID: %s@%s\r\n"
                                         "CSeq: 1 INVITE\r\n"
                                         "Contact: <sip:caller@%s>\r\n"
                                         "Content-Type: application/sdp\r\n"
                                         "Content-Length: %d\r\n"
                                         "\r\n"
                                         "%s";

// SDP 消息模板
const std::string SDP_TEMPLATE = "v=0\r\n"
                                 "o=- %d 0 IN IP4 %s\r\n"
                                 "s=-\r\n"
                                 "c=IN IP4 %s\r\n"
                                 "t=0 0\r\n"
                                 "m=audio %d RTP/AVP 0\r\n"
                                 "a=rtpmap:0 PCMU/8000\r\n";

// 生成随机的分支标识符
std::string generate_branch()
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", rand());
    return buf;
}

// 生成随机的 Call-ID
std::string generate_call_id()
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d.%d.%d",
             rand() % 256, rand() % 256, rand() % 256,
             rand() % 256, rand() % 256, rand() % 256);
    return buf;
}

// 发送 SIP 请求消息
void send_sip_request(int sock_fd, const std::string &callee_ip, const std::string &callee_port)
{
    char buf[1024];
    std::string branch = generate_branch();
    std::string call_id = generate_call_id();
    snprintf(buf, sizeof(buf), SDP_TEMPLATE.c_str(), 0, "127.0.0.1", "127.0.0.1", 8000);

    char request_buf[4096];
    snprintf(request_buf, sizeof(request_buf), SIP_REQUEST_TEMPLATE.c_str(),
             callee_ip.c_str(), "127.0.0.1", branch.c_str(),
             "127.0.0.1", call_id.c_str(), callee_ip.c_str(),
             call_id.c_str(), "127.0.0.1", static_cast<int>(strlen(buf)), buf);
    std::string request = request_buf;

    int ret = send(sock_fd, request.c_str(), request.length(), 0);
    if (ret < 0)
    {
        std::cerr << "Failed to send SIP request\n";
        exit(1);
    }
}

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(5060);

    int ret = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        std::cerr << "Failed to bind socket\n";
        exit(1);
    }

    ret = listen(sock_fd, 5);
    if (ret < 0)
    {
        std::cerr << "Failed to listen on socket\n";
        exit(1);
    }

    std::cout << "Listening on port 5060...\n";

    while (true)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0)
        {
            std::cerr << "Failed to accept connection\n";
            exit(1);
        }

        char buf[1024];
        int len = recv(client_fd, buf, sizeof(buf), 0);
        if (len < 0)
        {
            std::cerr << "Failed to receive data\n";
            exit(1);
        }
        buf[len] = '\0';

        std::cout << "Received message:\n"
                  << buf << "\n";

        // 解析 SIP 请求消息，根据需要回复响应消息
        // ...

        close(client_fd);
    }

    close(sock_fd);

    return 0;
}
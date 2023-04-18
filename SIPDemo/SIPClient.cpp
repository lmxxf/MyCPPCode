#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const std::string SIP_REQUEST_TEMPLATE = "INVITE %s SIP/2.0\r\n"
                                         "Via: SIP/2.0/UDP 127.0.0.1:5060;rport;branch=%s\r\n"
                                         "Max-Forwards: 70\r\n"
                                         "From: <sip:%s@127.0.0.1>;tag=%s\r\n"
                                         "To: <sip:%s@127.0.0.1>\r\n"
                                         "Call-ID: %s@127.0.0.1\r\n"
                                         "CSeq: 1 INVITE\r\n"
                                         "Contact: <sip:%s@127.0.0.1:5060>\r\n"
                                         "Content-Type: application/sdp\r\n"
                                         "Content-Length: %d\r\n\r\n"
                                         "%s";

const std::string SDP_TEMPLATE = "v=0\r\n"
                                 "o=- 0 0 IN IP4 %s\r\n"
                                 "s=-\r\n"
                                 "c=IN IP4 %s\r\n"
                                 "t=0 0\r\n"
                                 "m=audio %d RTP/AVP 0\r\n";

std::string generate_branch()
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%x%x", rand() % 0x7FFF, rand() % 0x7FFF);
    return buf;
}

std::string generate_call_id()
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%x%x@127.0.0.1", rand() % 0x7FFF, rand() % 0x7FFF);
    return buf;
}

void send_sip_request(int sock_fd, const std::string &callee_ip, const std::string &call_id)
{
    char buf[1024];
    std::string branch = generate_branch();
    std::string sdp = SDP_TEMPLATE;
    snprintf(buf, sizeof(buf), sdp.c_str(), "127.0.0.1", "127.0.0.1", 8000);

    char request_buf[4096];
    snprintf(request_buf, sizeof(request_buf), SIP_REQUEST_TEMPLATE.c_str(),
             callee_ip.c_str(), branch.c_str(), "1000", call_id.c_str(), callee_ip.c_str(),
             call_id.c_str(), "1000", static_cast<int>(strlen(buf)), buf);
    std::string request = request_buf;

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5060);
    inet_pton(AF_INET, callee_ip.c_str(), &(server_addr.sin_addr));

    int ret = sendto(sock_fd, request.c_str(), request.length(), 0,
                     (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        std::cerr << "Failed to send SIP request\n";
        exit(1);
    }
}

int main()
{
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0)
    {
        std::cerr << "Failed to create socket\n";
        exit(1);
    }
    std::string callee_ip = "127.0.0.1";
    std::string call_id = generate_call_id();
    send_sip_request(sock_fd, callee_ip, call_id);

    char response_buf[4096];
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int ret = recvfrom(sock_fd, response_buf, sizeof(response_buf), 0,
                       (struct sockaddr *)&client_addr, &client_addr_len);
    if (ret < 0)
    {
        std::cerr << "Failed to receive SIP response\n";
        exit(1);
    }

    response_buf[ret] = '\0';
    std::cout << "Received SIP response:\n"
              << response_buf << std::endl;

    close(sock_fd);
    return 0;
}

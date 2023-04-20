#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int RTP_PORT = 5000;
const int RTCP_PORT = 5001;

struct RTPPacket {
    unsigned char version;
    unsigned char padding;
    unsigned char extension;
    unsigned char cc;
    unsigned char marker;
    unsigned char payload_type;
    unsigned short sequence_number;
    unsigned int timestamp;
    unsigned int ssrc;
    char payload[1024];
};

struct RTCPReport {
    unsigned char version;
    unsigned char padding;
    unsigned char report_count;
    unsigned char packet_type;
    unsigned short length;
    unsigned int ssrc;
    char payload[1024];
};

int main()
{
    // 创建 RTP socket
    int rtp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (rtp_fd == -1) {
        std::cerr << "Failed to create RTP socket." << std::endl;
        return -1;
    }

    // 设置地址重用选项，避免端口占用问题
    int opt = 1;
    if (setsockopt(rtp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Failed to set RTP socket options." << std::endl;
        return -1;
    }

    // 设置地址和端口
    struct sockaddr_in rtp_addr;
    rtp_addr.sin_family = AF_INET;
    rtp_addr.sin_addr.s_addr = INADDR_ANY;
    rtp_addr.sin_port = htons(RTP_PORT);

    // 绑定地址和端口
    if (bind(rtp_fd, (struct sockaddr *)&rtp_addr, sizeof(rtp_addr)) == -1) {
        std::cerr << "Failed to bind RTP address." << std::endl;
        return -1;
    }

    std::cout << "Server listening on RTP port " << RTP_PORT << std::endl;

    // 创建 RTCP socket
    int rtcp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (rtcp_fd == -1) {
        std::cerr << "Failed to create RTCP socket." << std::endl;
        return -1;
    }

    // 设置地址重用选项，避免端口占用问题
    if (setsockopt(rtcp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        std::cerr << "Failed to set RTCP socket options." << std::endl;
        return -1;
    }

    // 设置地址和端口
    struct sockaddr_in rtcp_addr;
    rtcp_addr.sin_family = AF_INET;
    rtcp_addr.sin_addr.s_addr = INADDR_ANY;
    rtcp_addr.sin_port = htons(RTCP_PORT);

    // 绑定地址和端口
    if (bind(rtcp_fd, (struct sockaddr *)&rtcp_addr, sizeof(rtcp_addr)) == -1) {
        std::cerr << "Failed to bind RTCP address." << std::endl;
        return -1;
    }

    std::cout << "Server listening on RTCP port " << RTCP_PORT << std::endl;

    // 循环处理客户端请求
    while (true) {
        // 接收 RTP 包
        RTPPacket packet;
        struct sockaddr_in client_rtp_addr;
        socklen_t addr_len = sizeof(client_rtp_addr);
        if (recvfrom(rtp_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_rtp_addr, &addr_len) == -1) {
            std::cerr << "Failed to receive RTP packet." << std::endl;
            continue;
        }

        // 输出 RTP 包信息
        std::cout << "Received RTP packet from " << inet_ntoa(client_rtp_addr.sin_addr) << ":" << ntohs(client_rtp_addr.sin_port) << std::endl;
        std::cout << "  version: " << (int)packet.version << std::endl;
        std::cout << "  padding: " << (int)packet.padding << std::endl;
        std::cout << "  extension: " << (int)packet.extension << std::endl;
        std::cout << "  cc: " << (int)packet.cc << std::endl;
        std::cout << "  marker: " << (int)packet.marker << std::endl;
        std::cout << "  payload_type: " << (int)packet.payload_type << std::endl;
        std::cout << "  sequence_number: " << ntohs(packet.sequence_number) << std::endl;
        std::cout << "  timestamp: " << ntohl(packet.timestamp) << std::endl;
        std::cout << "  ssrc: " << ntohl(packet.ssrc) << std::endl;

        // 接收 RTCP 包
        RTCPReport report;
        struct sockaddr_in client_rtcp_addr;
        addr_len = sizeof(client_rtcp_addr);
        if (recvfrom(rtcp_fd, &report, sizeof(report), 0, (struct sockaddr *)&client_rtcp_addr, &addr_len) == -1) {
            std::cerr << "Failed to receive RTCP report." << std::endl;
            continue;
        }

        // 输出 RTCP 包信息
        std::cout << "Received RTCP report from " << inet_ntoa(client_rtcp_addr.sin_addr) << ":" << ntohs(client_rtcp_addr.sin_port) << std::endl;
        std::cout << "  version: " << (int)report.version << std::endl;
        std::cout << "  padding: " << (int)report.padding << std::endl;
        std::cout << "  report_count: " << (int)report.report_count << std::endl;
        std::cout << "  packet_type: " << (int)report.packet_type << std::endl;
        std::cout << "  length: " << ntohs(report.length) << std::endl;
        std::cout << "  ssrc: " << ntohl(report.ssrc) << std::endl;
    }

    // 关闭 RTP socket
    close(rtp_fd);

    // 关闭 RTCP socket
    close(rtcp_fd);

    return 0;

}
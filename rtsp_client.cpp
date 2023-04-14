#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const char* SERVER_IP = "127.0.0.1";
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

    // 设置地址和端口
    struct sockaddr_in rtp_addr;
    rtp_addr.sin_family = AF_INET;
    rtp_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    rtp_addr.sin_port = htons(RTP_PORT);

    std::cout << "Client sending to RTP port " << RTP_PORT << std::endl;

    // 创建 RTCP socket
    int rtcp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (rtcp_fd == -1) {
        std::cerr << "Failed to create RTCP socket." << std::endl;
        return -1;
    }

    // 设置地址和端口
    struct sockaddr_in rtcp_addr;
    rtcp_addr.sin_family = AF_INET;
    rtcp_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    rtcp_addr.sin_port = htons(RTCP_PORT);

    std::cout << "Client sending to RTCP port " << RTCP_PORT << std::endl;

    // 循环发送 RTP 和 RTCP 包
    for (int i = 0; i < 10; i++) {
        // 发送 RTP 包
        RTPPacket packet;
        std::memset(&packet, 0, sizeof(packet));
        packet.version = 2;
        packet.payload_type = 96;
        packet.sequence_number = htons(i);
        packet.timestamp = htonl(i * 1000);
        packet.ssrc = htonl(12345);
        std::sprintf(packet.payload, "RTP packet #%d", i);
        if (sendto(rtp_fd, &packet, sizeof(packet), 0, (struct sockaddr *)&rtp_addr, sizeof(rtp_addr)) == -1) {
            std::cerr << "Failed to send RTP packet." << std::endl;
        }

        // 发送 RTCP 包
        RTCPReport report;
        std::memset(&report, 0, sizeof(report));
        report.version = 2;
        report.report_count = 1;
        report.packet_type = 200;
        report.length = htons(6);
        report.ssrc = htonl(12345);
        std::sprintf(report.payload, "RTCP report #%d", i);
        if (sendto(rtcp_fd, &report, sizeof(report), 0, (struct sockaddr *)&rtcp_addr, sizeof(rtcp_addr)) == -1) {
            std::cerr << "Failed to send RTCP report." << std::endl;
        }
        // 延时 1 秒
        sleep(1);
    }

    // 关闭 RTP socket
    close(rtp_fd);

    // 关闭 RTCP socket
    close(rtcp_fd);

    return 0;
}
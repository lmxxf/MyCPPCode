#include <rte_eal.h>
#include <rte_ethdev.h>

int main(int argc, char **argv) {
    int ret;
    // 初始化 DPDK 环境
    ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot initialize EAL\n");
    }

    // 配置以太网接口
    struct rte_eth_conf port_conf = {
        .rxmode = { .max_rx_pkt_len = ETHER_MAX_LEN }
    };
    uint16_t port_id = 0;
    ret = rte_eth_dev_configure(port_id, 1, 1, &port_conf);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot configure Ethernet port %u\n", port_id);
    }

    // 启动以太网接口
    ret = rte_eth_dev_start(port_id);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Cannot start Ethernet port %u\n", port_id);
    }

    // 处理网络数据包
    while (true) {
        struct rte_mbuf *mbuf = NULL;
        // 接收数据包
        ret = rte_eth_rx_burst(port_id, 0, &mbuf, 1);
        if (ret > 0) {
            // 处理数据包
            // ...
            // 释放数据包缓存
            rte_pktmbuf_free(mbuf);
        }
    }

    // 停止以太网接口
    rte_eth_dev_stop(port_id);
    return 0;
}

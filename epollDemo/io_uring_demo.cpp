#include <iostream>
#include <fcntl.h>
#include <liburing.h>


int main() {
    // 创建 io_uring 对象
    struct io_uring ring;
    io_uring_queue_init(128, &ring, 0);

    // 打开文件
    int fd = open("file.txt", O_RDONLY);
    if (fd < 0) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    // 创建 I/O 操作请求
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, /*buf=*/nullptr, /*len=*/0, /*off=*/0);
    io_uring_submit(&ring);

    // 等待 I/O 操作完成
    struct io_uring_cqe *cqe;
    io_uring_wait_cqe(&ring, &cqe);
    if (cqe->res < 0) {
        std::cerr << "I/O error: " << strerror(-cqe->res) << std::endl;
        return 1;
    }
    std::cout << "Read " << cqe->res << " bytes from file\n";

    // 释放 I/O 操作请求和 I/O 完成事件
    io_uring_cqe_seen(&ring, cqe);
    io_uring_free_sqe(sqe);

    // 关闭文件和 io_uring 对象
    close(fd);
    io_uring_queue_exit(&ring);
    return 0;
}

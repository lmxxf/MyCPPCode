#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>

int main() {
    // 使用 syscall 函数调用 getpid 系统调用
    pid_t pid = syscall(SYS_getpid);
    std::cout << "Current process ID: " << pid << std::endl;
    return 0;
}

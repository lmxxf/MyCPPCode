#include <iostream>
#include <memory>

void foo(std::unique_ptr<int> p) {
    std::cout << "Inside foo: " << *p << std::endl;
}

int main() {
    std::unique_ptr<int> uptr(new int(42)); // uptr现在拥有一个动态分配的int对象
    // foo(uptr); // 编译错误：unique_ptr不支持拷贝
    foo(std::move(uptr)); // 将uptr的所有权转移到foo函数

    if (uptr) {
        std::cout << "uptr still owns the object." << std::endl;
    } else {
        std::cout << "uptr has lost ownership." << std::endl;
    }

    return 0;
}

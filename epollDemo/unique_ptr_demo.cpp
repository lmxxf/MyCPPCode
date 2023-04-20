// #include <iostream>
// #include <memory>

// void foo(std::unique_ptr<int> p) {
//     std::cout << "Inside foo: " << *p << std::endl;
// }

// int main() {
//     std::unique_ptr<int> uptr(new int(42)); // uptr现在拥有一个动态分配的int对象
//     // foo(uptr); // 编译错误：unique_ptr不支持拷贝
//     foo(std::move(uptr)); // 将uptr的所有权转移到foo函数

//     if (uptr) {
//         std::cout << "uptr still owns the object." << std::endl;
//     } else {
//         std::cout << "uptr has lost ownership." << std::endl;
//     }

//     return 0;
// }

#include <iostream>

template<typename T>
class my_unique_ptr {
public:
    my_unique_ptr() : ptr_(nullptr) {}
    explicit my_unique_ptr(T* ptr) : ptr_(ptr) {}
    ~my_unique_ptr() { delete ptr_; }
    my_unique_ptr(const my_unique_ptr&) = delete;
    my_unique_ptr& operator=(const my_unique_ptr&) = delete;
    my_unique_ptr(my_unique_ptr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    my_unique_ptr& operator=(my_unique_ptr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }
    T* operator->() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* get() const { return ptr_; }
private:
    T* ptr_;
};

int main() {
    my_unique_ptr<int> ptr1(new int(10));
    std::cout << *ptr1 << std::endl;

    my_unique_ptr<int> ptr2;
    ptr2 = std::move(ptr1);
    std::cout << *ptr2 << std::endl;

    my_unique_ptr<int> ptr3 = std::move(ptr2);
    std::cout << *ptr3 << std::endl;

    return 0;
}

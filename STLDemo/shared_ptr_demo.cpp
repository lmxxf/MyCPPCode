#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "MyClass constructor" << std::endl; }
    ~MyClass() { std::cout << "MyClass destructor" << std::endl; }
};

void custom_deleter(MyClass* ptr) {
    std::cout << "Custom deleter called" << std::endl;
    delete ptr;
}

int main() {
    std::shared_ptr<MyClass> sp1(new MyClass()); // 使用 shared_ptr 的构造函数创建
    std::shared_ptr<MyClass> sp2 = std::make_shared<MyClass>(); // 使用 make_shared 创建
    std::shared_ptr<MyClass> sp3(new MyClass(), custom_deleter); // 使用自定义删除器
    return 0;
}

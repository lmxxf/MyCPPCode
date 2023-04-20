#include <iostream>
#include <new> // 包含 placement new 的头文件

class MyClass {
public:
    MyClass(int x) : value(x) {}
    void print() const { std::cout << "Value: " << value << std::endl; }

private:
    int value;
};

int main() {
    // 分配内存，足够容纳 MyClass 对象
    void* mem = ::operator new(sizeof(MyClass));

    // 使用 placement new 在已分配的内存中构造 MyClass 对象
    MyClass* obj = new (mem) MyClass(42);

    // 使用对象
    obj->print();

    // 手动调用对象的析构函数
    obj->~MyClass();

    // 释放内存
    ::operator delete(mem);

    return 0;
}

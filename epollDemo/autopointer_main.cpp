#include <iostream>
#include <memory>
using namespace std;

class MyClass {
public:
    MyClass() {
        cout << "MyClass constructor called" << endl;
    }

    ~MyClass() {
        cout << "MyClass destructor called" << endl;
    }

    void print() {
        cout << "Hello, World!" << endl;
    }
};

int main() {
    // 使用 std::unique_ptr 管理动态内存
    unique_ptr<MyClass> ptr1(new MyClass);
    ptr1->print();

    // 将所有权移动到新指针中
    unique_ptr<MyClass> ptr2 = std::move(ptr1);
    ptr2->print();

    // 使用 std::shared_ptr 共享所有权
    shared_ptr<MyClass> ptr3 = make_shared<MyClass>();
    ptr3->print();

    // 将所有权共享给另一个指针
    shared_ptr<MyClass> ptr4 = ptr3;
    ptr4->print();
    ptr3->print();

    MyClass *ptr5 = ptr3.get();

    return 0;
}

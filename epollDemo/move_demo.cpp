#include <iostream>
#include <vector>
using namespace std;

class MyClass {
public:
    void print(string procedure) {
        cout << "MyClass " << procedure <<" called " << this << " data=" << data << endl;
    }
    MyClass() {
        print("constructor");
        // 分配内存
        data = new int[10000000];
    }

    ~MyClass() {
        print("destructor");
        // 释放内存
        delete[] data;
    }

    // 移动构造函数
    MyClass(MyClass&& other) : data(other.data) {
        print("move constructor");
        other.data = nullptr;
    }

private:
    int* data;
};

int main() {
    // 理解“右值引用是左值”
    int &&rvalue_ref = 42; // rvalue_ref 是一个右值引用，绑定到一个右值（42）
    int &a = rvalue_ref; // rvalue_ref 在这里被当作左值，因为它有一个明确的内存位置
    cout << a << " " << rvalue_ref << endl;


    vector<MyClass> v;

    // 将临时对象移动到容器中
    v.push_back(MyClass());

    // 将容器中的对象移动到新对象中
    MyClass obj = std::move(v.back());
    
    // MyClass& obj2 = v.back();

    return 0;
}

// int main() {
//     MyClass obj;
//     MyClass obj2 = std:move(obj);

// }

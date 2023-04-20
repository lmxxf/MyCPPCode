#include <iostream>
// #include <string>
#include <vector>
#include <memory>
#include <algorithm>


class MyClass {
public:
    MyClass() { std::cout << "MyClass constructor called." << std::endl; }
    ~MyClass() { std::cout << "MyClass destructor called." << std::endl; }
    void hello() { std::cout << "Hello, world!" << std::endl; }
};

using namespace std;

void printVector(vector<int> &v) {
    cout << "\n" << endl;
    for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
}

void test1() {

    vector<int> v1;
    v1.push_back(10);
    v1.push_back(30);
    v1.push_back(20);
    v1.push_back(50);
    v1.push_back(40);
    v1.size();

    //迭代器
    vector<int>::iterator it = v1.begin();
    while (it != v1.end()) {
        cout << *it << " ";
        it++;
    }
    sort(v1.begin(), v1.end());
    printVector(v1);

}

void test2() {
    vector<int> v1;
    v1.reserve(1000); //预留空间
    vector<int>::iterator it = v1.begin();
    int realloc_count = 0;
    for (size_t i = 0; i < 1000; i++)
    {
        /* code */
        v1.push_back(i);
        if (it != v1.begin()) {
            realloc_count++;
            it = v1.begin();
            cout << realloc_count << " v1 开辟了新的空间" << " " << v1.capacity() <<endl;
        }
    }
    
}

void test3() {
    vector<int> *v1 = new vector<int>(5, 100);
    v1->reserve(1000);
    cout << v1->capacity() << " / " << v1->size() << endl;
    if (v1->empty()) {
        cout << "v1 is empty" << endl;
    } else {
        cout << "v1 is not empty" << endl;
    }
    vector<int>(*v1).swap(*v1);
    cout << v1->capacity() << " / " << v1->size() << endl;

    for (size_t i = 0; i < v1->size(); i++)
    {
        /* code */
        cout << v1->at(i) << " ";
    }
    delete v1;

    // unique_ptr<vector<int>> v2(new vector<int>(5, 100));
    // unique_ptr<vector<int>> v2 = make_unique<vector<int>>(5, 100);
    std::unique_ptr<MyClass> ptr(new MyClass());
    std::unique_ptr<MyClass> ptr2 = move(ptr);
    ptr->hello();

    
}

int main() {
    test1();

    // test2();
    // test3();

    // vector<int> v1(5, 10);
    // vector<int> v2(v1.begin(), v1.end());
    // vector<int> v3(v1);
    // vector<int> v4(5);
    // vector<int> v5{1, 2, 3, 4, 5};
    // vector<int> v6 = {1, 2, 3, 4, 5};
    // vector<int> v7;
    // v7 = {1, 2, 3, 4, 5};

    // cout << "v1 = ";
    // for (auto i : v1)
    //     cout << i << " ";
    // cout << endl;

    // cout << "v2 = ";
    // for (auto i : v2)
    //     cout << i << " ";
    // cout << endl;

    // cout << "v3 = ";
    // for (auto i : v3)
    //     cout << i << " ";
    // cout << endl;

    // cout << "v4 = ";
    // for (auto i : v4)
    //     cout << i << " ";
    // cout << endl;

    // cout << "v5 = ";
    // for (auto i : v5)
    //     cout << i << " ";
    // cout << endl;

    // cout << "v6 = ";
    // for (auto i : v6)
    //     cout << i << " ";
    // cout << endl;

    // cout << "v7 = ";
    // for (auto i : v7)
    //     cout << i << " ";
    // cout << endl;

    return 0;
}
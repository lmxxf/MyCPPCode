#include <iostream>
using namespace std;
class movedemo{
public:
    // 在C++中，类的构造函数可以使用冒号:来初始化成员变量和基类对象。冒号后面的内容称为初始化列表（Initialization List），它可以对成员变量和基类对象进行初始化赋值操作。
    movedemo():num(new int(0)){
        cout<<"construct!"<<endl;
    }
    //拷贝构造函数
    movedemo(const movedemo &d):num(new int(*d.num)){
        cout<<"copy construct!"<<endl;
    }
    //移动构造函数
    movedemo(movedemo &&d):num(d.num){
        d.num = NULL;
        cout<<"move construct!"<<endl;
    }
public:     //这里应该是 private，使用 public 是为了更方便说明问题
    int *num;
};
int main(){
    movedemo demo;
    cout << "demo2:\n";
    movedemo demo2 = demo;
    //cout << *demo2.num << endl;   //可以执行
    cout << "demo3:\n";
    movedemo demo3 = std::move(demo);
    //此时 demo.num = NULL，因此下面代码会报运行时错误
    //cout << *demo.num << endl;
    return 0;
}
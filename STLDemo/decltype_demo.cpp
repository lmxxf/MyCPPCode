#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

// using namespace std;

// 尾置返回允许我们在参数列表之后声明返回类型
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    // 处理序列
    return *beg;    // 返回序列中一个元素的引用
}
// 为了使用模板参数成员，必须用 typename
template <typename It>
auto fcn2(It beg, It end) -> typename std::remove_reference<decltype(*beg)>::type
{
    // 处理序列
    return *beg;    // 返回序列中一个元素的拷贝
}

// 不使用尾置返回类型
template <typename It>
decltype(*std::declval<It>()) fcn_a(It beg, It end)
{
    return *beg;
}

template <typename It>
typename std::remove_reference<decltype(*std::declval<It>())>::type fcn2_a(It beg, It end)
{
    return *beg;
}

int main() {
    std::vector<int> v = {1, 2, 3};

    auto x = fcn(v.begin(), v.end());
    auto y = fcn2(v.begin(), v.end());

    std::cout << "x, y=" << x << " " << y << std::endl;

    auto z = fcn_a(v.begin(), v.end());
    auto w = fcn2_a(v.begin(), v.end());
    std::cout << "z, w=" << z << " " << w << std::endl;

    // using T = int &;
    // T& r1;  // int& & r1 -> int& r1
    // T&& r2; // int& && r2 -> int& r2
    
    // using U = int &&;
    // U& r3;  // int&& & r3 -> int& r3
    // U&& r4; // int&& && r4 -> int&& r4


    return 0;
}
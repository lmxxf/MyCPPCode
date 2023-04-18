#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 使用Lambda表达式遍历数组并打印每个元素
    std::for_each(vec.begin(), vec.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << std::endl;

    // 使用Lambda表达式对数组元素进行平方操作
    std::transform(vec.begin(), vec.end(), vec.begin(), [](int x) {
        return x * x;
    });

    // 使用Lambda表达式遍历数组并打印每个元素
    std::for_each(vec.begin(), vec.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << std::endl;

    return 0;
}

#include <iostream>
#include <tuple>

int main() {
    // 创建一个三元组
    std::tuple<int, std::string, float> my_three_tuple(42, "hello", 3.14f);
    
    // 创建一个四元组
    std::tuple<int, std::string, float, bool> my_four_tuple(42, "hello", 3.14f, true);

    // 访问三元组的元素
    std::cout << "Three-tuple: "
              << std::get<0>(my_three_tuple) << ", "
              << std::get<1>(my_three_tuple) << ", "
              << std::get<2>(my_three_tuple) << std::endl;

    // 访问四元组的元素
    std::cout << "Four-tuple: "
              << std::get<0>(my_four_tuple) << ", "
              << std::get<1>(my_four_tuple) << ", "
              << std::get<2>(my_four_tuple) << ", "
              << std::get<3>(my_four_tuple) << std::endl;

    return 0;
}

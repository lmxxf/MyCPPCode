// #include <iostream>
// #include <vector>
// #include <algorithm>

// int main() {
//     std::vector<int> v = {1, 2, 3, 4, 5};

//     // Define a lambda function to multiply each element by 2
//     auto double_func = [](int x) { return x * 2; };

//     // Use std::transform algorithm with the lambda function
//     std::transform(v.begin(), v.end(), v.begin(), double_func);

//     // Print the modified vector
//     for (auto x : v) {
//         std::cout << x << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5};

    // Define a lambda function to multiply each element by 2
    // auto double_func = [](int x) { return x * 2; };

    auto double_func = [](int x) -> int { 
        int result = x * 2; 
        return result; 
    };

    // Use std::transform algorithm with the lambda function
    std::transform(v.begin(), v.end(), v.begin(), double_func);

    // Print the modified vector
    for (auto x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}

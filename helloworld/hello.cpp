#include <iostream>


int main(int argc, char* argv[])
{
    std::cout << "The program name is: " << argv[0] << std::endl;

    if (argc > 1) {
        std::cout << "The program arguments are: ";
        for (int i = 1; i < argc; ++i) {
            std::cout << argv[i] << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No program arguments." << std::endl;
    }
    // 调用函数
    
    
    return 0;
}
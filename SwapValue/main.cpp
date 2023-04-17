#include "Swap.h"
#include <iostream>

int main(int argc, char *argv[]) {
    int a = 5;
    int b = 10;
    std::cout << "The program name is: " << argv[0] << std::endl;
    if (argc > 1) {
        std::cout << "The program arguments are: ";
        for (int i = 1; i < argc; ++i) {
            std::cout << argv[i] << " ";
            if (i == 1) {
                a = std::stoi(argv[i]);
            } else if (i == 2) {
                b = std::stoi(argv[i]);
            }
        }
        std::cout << std::endl;
    } else {
        std::cout << "No program arguments." << std::endl;
    }


    Swap<int> s(a, b);
    std::cout << "Before swap: " << s.getA() << " " << s.getB() << std::endl;
    s.swapValues();
    std::cout << "After swap: " << s.getA() << " " << s.getB() << std::endl;
    return 0;
}

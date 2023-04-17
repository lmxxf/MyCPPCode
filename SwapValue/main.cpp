#include "Swap.h"
#include <iostream>

int main() {
    Swap<int> s(5, 10);
    std::cout << "Before swap: " << s.getA() << " " << s.getB() << std::endl;
    s.swapValues();
    std::cout << "After swap: " << s.getA() << " " << s.getB() << std::endl;
    return 0;
}

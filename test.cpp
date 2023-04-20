#include <iostream>
#include <exception>
using namespace std;

class DemoException: public std::exception
{
    const char* msg_;
public:
    DemoException(const char* msg) : msg_(msg) {}
    virtual const char* what() // const throw() 
    { 
        return msg_; 
    }
};


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

    try {
        throw DemoException("DemoException");
    } catch (exception& e) {
        std::cout << "DemoException: " << e.what() << std::endl;
    }

    return 0;
}
#include <iostream>
#include <string>

class Person {
public:
    Person(const std::string& name, int age) : name_(name), age_(age) {}

    // Getter methods
    const std::string& name() const { return name_; }
    int age() const { return age_; }

private:
    std::string name_;
    int age_;
};

// Overload the insertion operator <<
std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << "Name: " << person.name() << ", Age: " << person.age();
    return os;
}

int main() {
    Person alice("Alice", 30);
    std::cout << alice << std::endl; // Output: Name: Alice, Age: 30
    return 0;
}

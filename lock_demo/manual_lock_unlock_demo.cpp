#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
int shared_data = 0;

void increment_data() {
    for (int i = 0; i < 10000; ++i) {
        mtx.lock();
        ++shared_data;
        mtx.unlock();
    }
}

int main() {
    std::thread t1(increment_data);
    std::thread t2(increment_data);

    t1.join();
    t2.join();

    std::cout << "Shared data: " << shared_data << std::endl;

    return 0;
}

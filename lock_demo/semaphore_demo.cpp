#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>

constexpr int MAX_CONCURRENCY = 3;
int available_resources = MAX_CONCURRENCY;
std::mutex mtx;
std::condition_variable cv;

void acquire() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return available_resources > 0; });
    --available_resources;
}

void release() {
    std::unique_lock<std::mutex> lock(mtx);
    ++available_resources;
    cv.notify_one();
}

void worker(int id) {
    // 等待信号量可用
    acquire();

    // 保证同时运行的线程数量受到限制
    std::cout << "Worker " << id << " started." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Worker " << id << " finished." << std::endl;

    // 释放信号量
    release();
}

int main() {
    std::vector<std::thread> threads;

    // 创建 6 个工作线程
    for (int i = 0; i < 6; ++i) {
        threads.emplace_back(worker, i);
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

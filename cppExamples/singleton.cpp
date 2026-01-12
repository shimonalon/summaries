#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

/**
 * Task: Implement a thread-safe Singleton class.
 * Requirement: 
 *   - The class should only be instantiable once.
 *   - Getting the instance must be thread-safe.
 */
class Logger {
public:
    static Logger& getInstance() {
    /*
    According to c++11 standart:
    If control enters the declaration concurrently while the variable is being initialized,
    the concurrent execution shall wait for completion of the initialization.
    */
        static Logger instance;
        return instance;
    }
    
    void log(const std::string& message) const {
        std::cout << "[LOG]: " << message << std::endl;
    }

    // Delete copy-ctor and assignment to prevent duplicates
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() {
        std::cout << "Logger initialized." << std::endl;
    }
};

void thread_task(int id) {
    // Each thread tries to get the instance and log something
    Logger::getInstance().log("Thread " + std::to_string(id) + " access");
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 900; ++i) {
        threads.emplace_back(thread_task, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    return 0;
}

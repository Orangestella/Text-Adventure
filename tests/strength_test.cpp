#include <atomic>
#include <iostream>
#include <thread>
#include <vector>
#include "State.h"

int main() {
    for (int round = 0; round < 100; ++round) {
        State state(nullptr);
        std::atomic<bool> ready(false);
        std::vector<std::thread> workers;
        for (int i = 0; i < 8; ++i) {
            workers.emplace_back([&] {
                while (!ready.load()) std::this_thread::yield();
                for (int step = 0; step < 10; ++step) state.changeStrength(-1);
            });
        }
        ready.store(true);
        for (auto& worker : workers) worker.join();
        if (state.getStrength() != 20) {
            std::cerr << "Concurrent strength updates were lost.\n";
            return 1;
        }
        state.changeStrength(1000);
        if (state.getStrength() != 100) return 1;
        state.changeStrength(-1000);
        if (state.getStrength() != 0) return 1;
    }
    std::cout << "100 concurrent strength rounds passed.\n";
}

#include "live_reloader.hpp"
#include <iostream>
#include <thread>
#include <chrono>

typedef void (*PrintFn)();

int main() {
    LiveReloader reloader;

    std::cout << "Starting application...\n";
    if (!reloader.watch_dll("./test_plugin.dll")) {
        std::cerr << "Failed to load test_plugin.dll. Build the project first!\n";
        return 1;
    }

    std::cout << "Watcher active. Modify test_plugin.cpp and rebuild to test live reloading.\n\n";

    for (int i = 0; i < 50; ++i) {
        reloader.update();

        auto fn = reloader.get_function<PrintFn>("print_message");
        if (fn) {
            fn();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
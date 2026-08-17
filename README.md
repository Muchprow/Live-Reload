# Live Reloader 🔄

A lightweight, zero-dependency C++17 library for Windows dynamic link library (DLL) hot-swapping. Designed for games, engine engines, and tools to accelerate development iteration times.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Features

- ⚡ **Instant Reloads**: Automatically detects file changes and reloads DLLs in milliseconds.
- 🔒 **Bypasses Windows File Locking**: Copies target binaries to temporary files to avoid process lockouts.
- 📦 **Zero Dependencies**: Pure C++17 and Win32 API (`ReadDirectoryChangesW`, `LoadLibraryA`).
- 🛠️ **CMake Ready**: Easy integration as a static library or submodule.

## Quick Start

```cpp
#include <live_reloader/live_reloader.hpp>
#include <iostream>

int main() {
    // Initialize watcher for your plugin DLL
    LiveReloader reloader("test_plugin.dll");

    while (true) {
        // Poll for modifications and auto-reload
        if (reloader.update()) {
            std::cout << "[LiveReloader] Plugin reloaded successfully!\n";
        }

        // Execute your hot-swappable logic
        auto print_func = reloader.get_function<void(*)()>("plugin_print");
        if (print_func) {
            print_func();
        }
    }
}

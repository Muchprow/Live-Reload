#include <iostream>

extern "C" __declspec(dllexport) void print_message() {
    std::cout << ">>> HOT RELOAD SUCCESS! Code version: 2 <<<\n";
}
#pragma once
#include <windows.h>
#include <string>
#include <filesystem>
#include <iostream>

class DllReloader {
public:
    DllReloader() = default;
    ~DllReloader() { unload(); }

    bool load(const std::string& original_dll_path) {
        original_path_ = original_dll_path;
        std::filesystem::path p(original_dll_path);
        temp_path_ = p.parent_path() / (p.stem().string() + "_temp" + p.extension().string());
        return reload();
    }

    void unload() {
        if (module_handle_) {
            FreeLibrary(module_handle_);
            module_handle_ = nullptr;
        }
        if (!temp_path_.empty() && std::filesystem::exists(temp_path_)) {
            std::error_code ec;
            std::filesystem::remove(temp_path_, ec);
        }
    }

    bool reload() {
        unload();
        if (!std::filesystem::exists(original_path_)) return false;

        std::error_code ec;
        std::filesystem::copy_file(original_path_, temp_path_, std::filesystem::copy_options::overwrite_existing, ec);
        if (ec) return false;

        module_handle_ = LoadLibraryA(temp_path_.string().c_str());
        return module_handle_ != nullptr;
    }

    template <typename T>
    T get_function(const std::string& func_name) {
        if (!module_handle_) return nullptr;
        return reinterpret_cast<T>(GetProcAddress(module_handle_, func_name.c_str()));
    }

private:
    std::string original_path_;
    std::filesystem::path temp_path_;
    HMODULE module_handle_{ nullptr };
};
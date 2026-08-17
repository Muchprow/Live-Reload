#pragma once
#include "file_watcher.hpp"
#include "dll_reloader.hpp"
#include <memory>
#include <atomic>
#include <mutex>
#include <filesystem>

class LiveReloader {
public:
    LiveReloader() = default;

    bool watch_dll(const std::string& dll_path) {
        dll_path_ = dll_path;
        if (!reloader_.load(dll_path_)) return false;

        std::filesystem::path p(dll_path_);
        std::string folder = p.parent_path().string();
        if (folder.empty()) folder = ".";
        std::string filename = p.filename().string();

        watcher_.start(folder, [this, filename](const std::string& changed_file) {
            if (changed_file.find(filename) != std::string::npos) {
                need_reload_ = true;
            }
        });

        return true;
    }

    void update() {
        if (need_reload_) {
            std::lock_guard<std::mutex> lock(mutex_);
            std::cout << "\n[HOT-RELOAD] File changes detected. Reloading DLL...\n";
            reloader_.reload();
            need_reload_ = false;
        }
    }

    template <typename T>
    T get_function(const std::string& func_name) {
        std::lock_guard<std::mutex> lock(mutex_);
        return reloader_.get_function<T>(func_name);
    }

private:
    std::string dll_path_;
    FileWatcher watcher_;
    DllReloader reloader_;
    std::atomic<bool> need_reload_{false};
    std::mutex mutex_;
};
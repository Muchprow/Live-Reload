#pragma once
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <windows.h>

class FileWatcher {
public:
    using FileChangeCallback = std::function<void(const std::string& filepath)>;

    FileWatcher();
    ~FileWatcher();

    bool start(const std::string& directory_path, FileChangeCallback callback);
    void stop();

private:
    void watch_loop();

    std::string folder_path_;
    FileChangeCallback callback_;
    std::thread watch_thread_;
    std::atomic<bool> is_running_{false};
    HANDLE dir_handle_{INVALID_HANDLE_VALUE};
};
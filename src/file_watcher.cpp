#include "file_watcher.hpp"
#include <iostream>

FileWatcher::FileWatcher() = default;
FileWatcher::~FileWatcher() { stop(); }

bool FileWatcher::start(const std::string& directory_path, FileChangeCallback callback) {
    folder_path_ = directory_path;
    callback_ = callback;

    dir_handle_ = CreateFileA(
        folder_path_.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL
    );

    if (dir_handle_ == INVALID_HANDLE_VALUE) return false;

    is_running_ = true;
    watch_thread_ = std::thread(&FileWatcher::watch_loop, this);
    return true;
}

void FileWatcher::stop() {
    if (is_running_) {
        is_running_ = false;
        if (dir_handle_ != INVALID_HANDLE_VALUE) {
            CloseHandle(dir_handle_);
            dir_handle_ = INVALID_HANDLE_VALUE;
        }
        if (watch_thread_.joinable()) watch_thread_.join();
    }
}

void FileWatcher::watch_loop() {
    char buffer[1024];
    DWORD bytes_returned;

    while (is_running_) {
        if (ReadDirectoryChangesW(
                dir_handle_, buffer, sizeof(buffer), TRUE,
                FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME,
                &bytes_returned, NULL, NULL)) 
        {
            FILE_NOTIFY_INFORMATION* notify = (FILE_NOTIFY_INFORMATION*)buffer;
            while (notify) {
                if (notify->Action == FILE_ACTION_MODIFIED) {
                    std::wstring wname(notify->FileName, notify->FileNameLength / sizeof(WCHAR));
                    std::string filename(wname.begin(), wname.end());
                    if (callback_) callback_(filename);
                }
                if (notify->NextEntryOffset == 0) break;
                notify = (FILE_NOTIFY_INFORMATION*)((BYTE*)notify + notify->NextEntryOffset);
            }
        } else {
            break;
        }
    }
}
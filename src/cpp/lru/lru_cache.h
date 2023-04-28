#pragma once

#include <string>
#include <list>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <shared_mutex>

class LRUCache {
public:
    LRUCache(size_t max_size, std::chrono::seconds expiry);
    ~LRUCache();

    void Put(const std::string& key, const std::string& value);
    bool Get(const std::string& key, std::string& value);

private:
    void CacheCleanupTask();

    struct CacheEntry {
        std::string key;
        std::string value;
        std::chrono::steady_clock::time_point timestamp;
    };

    size_t max_size_;
    std::chrono::seconds expiry_duration_;
    std::list<CacheEntry> cache_list_;
    std::unordered_map<std::string, std::list<CacheEntry>::iterator> cache_map_;
    std::shared_timed_mutex cache_mutex_;
    std::thread cache_cleanup_thread_;
    bool stop_cleanup_ = false;
};

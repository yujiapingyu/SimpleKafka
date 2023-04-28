#include "lru_cache.h"
#include <iostream>
#include <thread>

LRUCache::LRUCache(size_t max_size, std::chrono::seconds expiry)
    : max_size_(max_size), expiry_duration_(expiry) {
    cache_cleanup_thread_ = std::thread([this] { CacheCleanupTask(); });
}

LRUCache::~LRUCache() {
    stop_cleanup_ = true;
    cache_cleanup_thread_.join();
}

void LRUCache::Put(const std::string& key, const std::string& value) {
    std::unique_lock<std::shared_timed_mutex> lock(cache_mutex_);
    auto it = cache_map_.find(key);
    if (it != cache_map_.end()) {
        cache_list_.erase(it->second);
    }
    cache_list_.push_front({key, value, std::chrono::steady_clock::now()});
    cache_map_[key] = cache_list_.begin();

    if (cache_map_.size() > max_size_) {
        auto last_it = cache_list_.end();
        --last_it;
        cache_map_.erase(last_it->key);
        cache_list_.pop_back();
    }
}

bool LRUCache::Get(const std::string& key, std::string& value) {
    std::shared_lock<std::shared_timed_mutex> lock(cache_mutex_);
    auto it = cache_map_.find(key);
    if (it == cache_map_.end()) {
        return false;
    }

    auto list_it = it->second;
    if (std::chrono::steady_clock::now() - list_it->timestamp > expiry_duration_) {
        // Expired
        cache_list_.erase(list_it);
        cache_map_.erase(it);
        return false;
    }

    value = list_it->value;
    cache_list_.erase(list_it);
    cache_list_.push_front({key, value, std::chrono::steady_clock::now()});
    cache_map_[key] = cache_list_.begin();
    return true;
}

void LRUCache::CacheCleanupTask() {
    while (!stop_cleanup_) {
        std::this_thread::sleep_for(expiry_duration_ / 2);
        std::unique_lock<std::shared_timed_mutex> lock(cache_mutex_);
        auto it = cache_list_.begin();
        while (it != cache_list_.end()) {
            if (std::chrono::steady_clock::now() - it->timestamp > expiry_duration_) {
                cache_map_.erase(it->key);
                it = cache_list_.erase(it);
            } else {
                ++it;
            }
        }
    }
}

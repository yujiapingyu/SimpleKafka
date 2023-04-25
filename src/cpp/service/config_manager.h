#pragma once

#include <string>
#include <nlohmann/json.hpp>

class ConfigManager {
public:
    static ConfigManager& get_instance();

    nlohmann::json get_config() const;

    void load_config(const std::string& file_path);

private:
    ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    nlohmann::json config;
};

#include "config_manager.h"
#include <fstream>

ConfigManager& ConfigManager::get_instance() {
    static ConfigManager instance;
    return instance;
}

nlohmann::json ConfigManager::get_config() const {
    return config;
}

void ConfigManager::load_config(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + file_path);
    }

    file >> config;
}

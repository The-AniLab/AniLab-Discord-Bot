#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

class Dotenv {
public:
    // Load .env file
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open .env file");
        }

        std::string line;
        while (std::getline(file, line)) {
            parseLine(line);
        }

        file.close();
    }

    // Find key in variables
    bool has(const std::string& key) const {
        return variables_.find(key) != variables_.end();
    }

    // Get value from key
    std::string get(const std::string& key) const {
        if (!has(key)) {
            throw std::runtime_error("Undefined key: " + key);
        }

        return variables_.at(key);
    }

private:
    std::unordered_map<std::string, std::string> variables_;

    void parseLine(const std::string& line) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss >> std::ws, key, '=') && std::getline(iss >> std::ws, value)) {
            variables_[key] = value;
        }
    }
};
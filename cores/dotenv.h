/*
 * Copyright (C) 2023  The Anilab Development Team
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see https://www.gnu.org/licenses/.
 */

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
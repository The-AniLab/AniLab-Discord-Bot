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
 * 
 *                       *******************
 * This original code belongs to @harshfeudal
 * Under MIT license
 * 
 * Copyright (c) 2023 harshfeudal
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

class Dotenv {
public:
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open .env file");
        }

        std::string line;
        while (std::getline(file, line)) {
            if (isComment(line)) {
                continue;
            }
            expandVariables(line);
            parseLine(line);
        }

        file.close();
    }

    bool has(const std::string& key) const {
        return variables_.find(key) != variables_.end();
    }

    std::string get(const std::string& key) const {
        if (!has(key)) {
            throw std::runtime_error("Undefined key: " + key);
        }

        return variables_.at(key);
    }

private:
    std::unordered_map<std::string, std::string> variables_;

    bool isComment(const std::string& line) {
        return line.find("#") == 0;
    }

    void expandVariables(std::string& line) {
        std::size_t pos = 0;
        while ((pos = line.find('$', pos)) != std::string::npos) {
            if (pos + 1 < line.length()) {
                if (line[pos + 1] == '\\') {
                    line.erase(pos, 1);
                    ++pos;
                } else if (line[pos + 1] == '{') {
                    std::size_t endPos = line.find('}', pos + 2);
                    if (endPos != std::string::npos) {
                        std::string varName = line.substr(pos + 2, endPos - pos - 2);
                        std::string varValue = (has(varName)) ? get(varName) : "";
                        line.replace(pos, endPos - pos + 1, varValue);
                        pos += varValue.length();
                    } else {
                        ++pos;
                    }
                } else {
                    std::string varName = line.substr(pos + 1);
                    std::string varValue = (has(varName)) ? get(varName) : "";
                    line.replace(pos, varName.length() + 1, varValue);
                    pos += varValue.length();
                }
            } else {
                break;
            }
        }
    }

    void parseLine(const std::string& line) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss >> std::ws, key, '=') && std::getline(iss >> std::ws, value)) {
            variables_[key] = value;
        }
    }
};
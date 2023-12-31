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

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <string>
#include <sqlite3/sqlite3.h>

#pragma warning(disable: 4715)

class DatabaseHandler 
{
public:
    DatabaseHandler() : database(nullptr) { }

    void open(const std::string& filename) 
    {
        int rc = sqlite3_open(filename.c_str(), &database);
        if (rc != SQLITE_OK)
            throw std::runtime_error("Can't open database: " + std::string(sqlite3_errmsg(database)));
        
        executeQuery("PRAGMA synchronous=OFF;");
        db_filename = filename;
    }

    // "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns + ");"
    void createTable(const std::string& tableName, const std::string& columns) 
    {
        std::string query = "CREATE TABLE IF NOT EXISTS " + tableName + " (" + columns + ");";
        executeQuery(query);
    }

    void executeQuery(const std::string& query) 
    {
        char* error_message = nullptr;
        int rc = sqlite3_exec(database, query.c_str(), nullptr, 0, &error_message);

        if (rc != SQLITE_OK) 
        {
            std::string errorMessage = "SQL error: " + std::string(error_message);
            sqlite3_free(error_message);
            throw std::runtime_error(errorMessage);
        }
    }

    // std::string query = "INSERT INTO " + tableName + " VALUES (" + values + ");"
    void insert(const std::string& tableName, const std::string& values) 
    {
        std::string query = "INSERT INTO " + tableName + " VALUES (" + values + ");";
        executeQuery(query);
    }

    // "DELETE FROM " + tableName + " WHERE " + condition + ";"
    void deleteRecord(const std::string& tableName, const std::string& condition) 
    {
        std::string query = "DELETE FROM " + tableName + " WHERE " + condition + ";";
        executeQuery(query);
    }

    // Find: "SELECT COUNT(*) FROM " + tableName + " WHERE " + searchCondition + ";"
    bool findRecord(const std::string& tableName, const std::string& searchCondition) 
    {
        std::string query = "SELECT COUNT(*) FROM " + tableName + " WHERE " + searchCondition + ";";
        int resultCount = 0;

        auto callback = [](void* data, int argc, char** argv, char** azColName) -> int
        {
            if (argc > 0 && argv[0] != nullptr)
            {
                auto& count = *static_cast<int*>(data);
                count = std::stoi(argv[0]);
            }

            return 0;
        };

        char* error_message = nullptr;
        int rc = sqlite3_exec(database, query.c_str(), callback, &resultCount, &error_message);

        if (rc != SQLITE_OK) 
        {
            std::string errorMessage = "SQL error: " + std::string(error_message);
            sqlite3_free(error_message);
            throw std::runtime_error(errorMessage);
        }

        return resultCount > 0;
    }

    // "SELECT " + targetColumn + " FROM " + tableName + " WHERE " + searchCondition + ";"
    std::string exportData(const std::string& tableName, const std::string& targetColumn, const std::string& searchCondition)
    {
        std::string query = "SELECT " + targetColumn + " FROM " + tableName + " WHERE " + searchCondition + ";";
        std::string result;

        auto callback = [](void* data, int argc, char** argv, char** azColName) -> int
        {
            if (argc > 0 && argv[0] != nullptr)
            {
                auto& result = *static_cast<std::string*>(data);
                result = argv[0];
            }

            return 0;
        };

        char* error_message = nullptr;
        int rc = sqlite3_exec(database, query.c_str(), callback, &result, &error_message);

        if (rc != SQLITE_OK) 
        {
            std::string errorMessage = "SQL error: " + std::string(error_message);
            sqlite3_free(error_message);
            throw std::runtime_error(errorMessage);
        }

        return result;
    }

    ~DatabaseHandler() 
    {
        if (database)
            sqlite3_close(database);
    }

private:
    sqlite3* database;
    std::string db_filename;
};

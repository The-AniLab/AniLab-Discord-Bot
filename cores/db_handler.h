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

    // "SELECT " + condition_1 + " FROM " + tableName + " WHERE " + condition_2 + ";"
    std::string read(const std::string& tableName, const std::string& condition_1, const std::string& condition_2)
    {
        std::string query = "SELECT " + condition_1 + " FROM " + tableName + " WHERE " + condition_2 + ";";

        char* error_message = nullptr;
        char** result = nullptr;
        int rows, columns;
        int rc = sqlite3_get_table(database, query.c_str(), &result, &rows, &columns, &error_message);

        if (rc != SQLITE_OK)
        {
            std::string errorMessage = "SQL error: " + std::string(error_message);
            sqlite3_free(error_message);
            sqlite3_free_table(result);
            throw std::runtime_error(errorMessage);
        }

        std::string value = "";
        if (rows > 0 && columns > 0)
        {
            int index = 1;
            value = result[index];
        }

        sqlite3_free_table(result);
        return value;
    }

    // std::string query = "INSERT INTO " + tableName + " VALUES (" + values + ");"
    void insert(const std::string& tableName, const std::string& values) 
    {
        std::string query = "INSERT INTO " + tableName + " VALUES (" + values + ");";
        executeQuery(query);
    }

    void deleteRecord(const std::string& tableName, const std::string& condition) 
    {
        std::string query = "DELETE FROM " + tableName + " WHERE " + condition + ";";
        executeQuery(query);
    }

    int findRecord(const std::string& tableName, const std::string& condition_1, const std::string& condition_2) 
    {
        std::string query = "SELECT " + condition_1 + " FROM " + tableName + " WHERE " + condition_2 + ";";

        auto callback = [](void* idPtr, int argc, char** argv, char** azColName) -> int
        {
            int* id = static_cast<int*>(idPtr);
            if (argc > 0 && argv[0] != nullptr)
            {
                std::istringstream iss(argv[0]);
                if (!(iss >> *id))
                    *id = -1;
            }

            return 0;
        };

        int id = -1;
        char* error_message = nullptr;
        int rc = sqlite3_exec(database, query.c_str(), callback, &id, &error_message);

        if (rc != SQLITE_OK) 
        {
            std::string errorMessage = "SQL error: " + std::string(error_message);
            sqlite3_free(error_message);
            throw std::runtime_error(errorMessage);
        }

        return id;
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

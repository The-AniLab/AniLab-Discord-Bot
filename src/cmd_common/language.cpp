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

#include <fstream>
#include <string>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <cmd_lists.h>
#include <db_handler.h>
#include <embed_builder.h>

using json = nlohmann::json;

void language(dpp::cluster& client, const dpp::slashcommand_t& event)
{
    const auto language = std::get<std::string>(event.get_parameter("option"));

    DatabaseHandler database;
    json language_config;

    database.open("./database/language.db");
    database.createTable("configuration", "id TEXT, language TEXT");

    auto target_user = fmt::format("'{}'", event.command.usr.id);
    auto check_user = database.findRecord("configuration", "id", target_user);

    if (language == "english")
    {
        // If find, delete it because English is default
        if (check_user != -1)
            database.deleteRecord("configuration", "id=" + target_user);
    }
    else if (language == "japanese")
    {
        // If find, delete the old one and create new
        if (check_user != -1)
            database.deleteRecord("configuration", target_user);

        database.insert("configuration", target_user + ", 'ja-jp'");
    }
}

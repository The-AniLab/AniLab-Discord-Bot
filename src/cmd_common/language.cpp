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

using json = nlohmann::json;

void language(dpp::cluster& client, const dpp::slashcommand_t& event)
{
    const auto language = std::get<std::string>(event.get_parameter("option"));

    DatabaseHandler database;
    database.open("./database/language.db");

    const auto find_user = fmt::format("id = '{}'", event.command.usr.id);

    if (language == "english")
    {
        auto status = database.findRecordID("configuration", find_user);

        if (status != -1)
            database.deleteRecord("configuration", find_user);

        // Test only - Not a final code result
        json language_config;
        std::ifstream language_file("./languages/en-us.json");
    
        language_file >> language_config;
        auto language_embed = language_config["LANGUAGE"];
    
        auto create_embed = dpp::embed()
	        .set_title(language_embed["title"])
	        .set_color(0x38ff9b)
	        .set_description(language_embed["description"])
	        .set_timestamp(time(0));
    
	    event.reply(dpp::message().add_embed(create_embed));
    }
}
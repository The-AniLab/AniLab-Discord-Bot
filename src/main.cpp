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

#pragma warning(disable: 4251)
#pragma warning(disable: 6385)
#pragma warning(disable: 6285)

#include <dpp/dpp.h>
#include <spdlog/spdlog.h>

#include <dotenv.h>
#include <decoder.h>
#include <button.h>
#include <cmd_config.h>

int main() 
{
    Dotenv dotenv;
    dotenv.load(".env");

    const auto encodeToken = dotenv.get("ENCODE_CLIENT_TOKEN");
    const auto token = decode(encodeToken);

    dpp::cluster client(token, dpp::i_all_intents);
    client.on_log(dpp::utility::cout_logger());
    
    client.on_ready([&client](const dpp::ready_t& event)
        {
            client.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_watching, "AniLab"));
            fmt::print("[{}]: {} is online!\n", dpp::utility::current_date_time(), client.me.format_username());

            try
            {
                slashCommandCreate(client);
                fmt::print("[{}]: Successfully registered application (/) commands!\n", dpp::utility::current_date_time());
            }
            catch (...) { }
        });

    client.on_button_click([](const dpp::button_click_t& event) { buttonBuilder(event); });

    client.on_slashcommand([&client](const dpp::slashcommand_t& event)
		{
			dpp::command_interaction data = event.command.get_command_interaction();
			auto filter = commands.find(data.name);

			if (filter != commands.end())
				filter->second.function(client, event);
		});

    SetConsoleTitle(TEXT("Anilab Discord Bot"));

    client.start(dpp::st_wait);
    return 0;
}
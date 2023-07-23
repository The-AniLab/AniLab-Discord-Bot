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

#include <map>

#include <dpp/dpp.h>

#include <cmd_struct.h>
#include <cmd_lists.h>

inline std::map<std::string, Command> commands
{ 
    // Common commands
    {
        "ping",
        {
            "Check bot latency", ping 
        }
    },
    {
        "language",
        {
            "Configure bot language to show to you", language,
            {
                dpp::command_option(dpp::co_string, "option", "Your language option", true)
                    .add_choice(dpp::command_option_choice("English", std::string("english")))
                    .add_choice(dpp::command_option_choice("日本語", std::string("japanese")))
                    .add_choice(dpp::command_option_choice("Tiếng Việt", std::string("vietnamese")))
            }
        }
    },

    // miHoYo commands
    {
        "login",
        {
            "Login into miHoYo account", login
        }
    },
};
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

#pragma warning (disable: 4251)
#pragma warning (disable: 4244)

#include <dpp/dpp.h>

// Common commands
void ping(dpp::cluster& client, const dpp::slashcommand_t& event);

// Moderation commands
// ...
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

using json = nlohmann::json;

void ping(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	const auto ws_ping = fmt::format("`{0:.02f} ms`", event.from->websocket_ping * 1000);
	const auto rest_ping = fmt::format("`{0:.02f} ms`", event.from->creator->rest_ping * 1000);
	
	json language_config;
	std::ifstream language("./languages/en-us.json");

	language >> language_config;
	auto ping_embed = language_config["PING"];

	auto create_embed = dpp::embed()
		.set_title(ping_embed["title"])
		.set_color(0x9dc3f8)
		.add_field(ping_embed["websocket"], ws_ping, true)
		.add_field(ping_embed["roundtrip"], rest_ping, true)
		.set_thumbnail(client.me.get_avatar_url(1024, dpp::i_webp))
		.set_footer(
			dpp::embed_footer()
				.set_text(client.me.format_username())
				.set_icon(client.me.get_avatar_url()
			)
		)
		.set_timestamp(time(0));

	event.reply(dpp::message().add_embed(create_embed));
}
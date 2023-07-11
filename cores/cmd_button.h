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
#pragma warning(disable: 4101)

#include <dpp/dpp.h>

#define ID_SPACING "###"

struct Session
{
	time_t created_at;
	uint16_t cache_duration;

	std::function<bool(const dpp::button_click_t&)> function;

	Session() : created_at(time(nullptr)), cache_duration(10) {}

	bool isExpired() const
	{
		return difftime(time(nullptr), created_at) > cache_duration * 60;
	}
};

inline std::unordered_map<uint64_t, Session> cached_sessions;
inline std::shared_mutex cached_sessions_mutex;
inline uint64_t custom_id_counter;

inline void clear()
{
	std::unique_lock l(cached_sessions_mutex);

	auto it = cached_sessions.begin();

	// Time check button to clear
	while (it != cached_sessions.end())
	{
		if (it->second.isExpired())
		{
			assert(!cached_sessions.empty());
			it = cached_sessions.erase(it);
		}
		else ++it;
	}
}

inline void button_bind(dpp::component& component, const std::function<bool(const dpp::button_click_t&)>& function, uint16_t cache_duration = 10)
{
	clear();

	std::unique_lock l(cached_sessions_mutex);
	bool custom_id_already_exists;

	do
	{
		// Checking initial button create
		if (custom_id_counter >= UINT_LEAST64_MAX)
			custom_id_counter = 0;

		custom_id_counter++;
		custom_id_already_exists = cached_sessions.find(custom_id_counter) != cached_sessions.end();

		// Checking if existence
		if (!custom_id_already_exists)
		{
			component.custom_id = std::to_string(custom_id_counter);

			Session session;

			session.function = function;
			session.cache_duration = cache_duration;

			component.custom_id += ID_SPACING + std::to_string(static_cast<long int>(session.created_at));

			cached_sessions[custom_id_counter] = session;
			custom_id_already_exists = false;
		}
	} while (custom_id_already_exists);
}

inline void button_builder(const dpp::button_click_t& event)
{
	uint64_t custom_id;
	time_t creation_timestamp;

	try
	{
		const std::string id = event.custom_id.substr(0, event.custom_id.find(ID_SPACING));
		const std::string creation = event.custom_id.substr(event.custom_id.find(ID_SPACING) + std::strlen(ID_SPACING), std::string::npos);

		custom_id = std::stoul(id);
		creation_timestamp = std::stol(creation);
	}
	catch (std::out_of_range& e)
	{
		event.reply(
			dpp::message().set_content("Button not found!")
			.set_flags(dpp::m_ephemeral)
		);

		return;
	}
	catch (std::invalid_argument& e)
	{
		event.reply(
			dpp::message().set_content("Button not found!")
			.set_flags(dpp::m_ephemeral)
		);

		return;
	}

	std::unique_lock l(cached_sessions_mutex);
	const auto existing = cached_sessions.find(custom_id);

	if (existing != cached_sessions.end() && existing->second.created_at == creation_timestamp && !existing->second.isExpired())
	{
		bool forget = existing->second.function(event);

		// If forget the button
		if (forget)
		{
			cached_sessions.erase(existing);
		}
	}
}
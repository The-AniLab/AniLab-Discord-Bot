#pragma once

#include <map>

#include <cmd_layout.h>

inline void slashCommandCreate(dpp::cluster& client)
{
	if (dpp::run_once<struct register_commands>())
	{
		std::vector<dpp::slashcommand> slashcommand;

		for (auto& def : commands)
		{
			dpp::slashcommand cmd;
			cmd.set_name(def.first).set_description(def.second.description).set_application_id(client.me.id);
			cmd.options = def.second.param;

			slashcommand.push_back(cmd);
		}

		client.global_bulk_command_create(slashcommand);
	}
}
#pragma once

#include <functional>

#include <dpp/dpp.h>

using  command_function = std::function<void(dpp::cluster&, const dpp::slashcommand_t&)>;
struct command_definition;

struct Command
{
	std::string description;
	command_function function;

	std::vector<dpp::command_option> param = {};
};
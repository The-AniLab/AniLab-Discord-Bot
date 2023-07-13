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
	std::ifstream language("../../languages/en-us.json");

	language >> language_config;
	auto ping_embed = language_config["PING"];

	auto create_embed = dpp::embed()
		.set_color(0x9dc3f8)
		.add_field(ping_embed["websocket"], ws_ping)
		.add_field(ping_embed["roundtrip"], rest_ping)
		.set_thumbnail(client.me.get_avatar_url())
		.set_timestamp(time(0));

	event.reply(dpp::message().add_embed(create_embed));
}
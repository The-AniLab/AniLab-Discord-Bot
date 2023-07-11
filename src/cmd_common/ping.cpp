#include <spdlog/spdlog.h>

#include <cmd_lists.h>

void ping(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	const auto ws_ping = fmt::format("`{0:.02f} ms`", event.from->websocket_ping * 1000);
	const auto rest_ping = fmt::format("`{0:.02f} ms`", event.from->creator->rest_ping * 1000);

    const auto embed_color = client.user_get_sync(client.me.id).accent_color;
	const auto content = fmt::format("Average Websocket Ping: {}\nRoundtrip latency: {}", ws_ping, rest_ping);

	auto create_embed = dpp::embed()
		.set_color(embed_color)
		.set_description(content)
		.set_footer(
			dpp::embed_footer()
			.set_text(client.me.format_username())
			.set_icon(client.me.get_avatar_url())
		)
		.set_timestamp(time(0));

	event.reply(dpp::message().add_embed(create_embed));
}
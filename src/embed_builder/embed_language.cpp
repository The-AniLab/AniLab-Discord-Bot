#include <embed_builder.h>

dpp::embed embed_language(json language_config)
{
	auto language_embed = language_config["LANGUAGE"];
    auto create_embed = dpp::embed()
	    .set_title(language_embed["title"])
	    .set_color(0x38ff9b)
	    .set_description(language_embed["description"])
	    .set_timestamp(time(0));
}
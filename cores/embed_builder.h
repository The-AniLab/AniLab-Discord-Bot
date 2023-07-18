#pragma once

#include <dpp/dpp.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

dpp::embed embed_language(json language_embed);
#pragma once

#pragma warning (disable: 4251)
#pragma warning (disable: 4244)

#include <dpp/dpp.h>

// Common commands
void ping(dpp::cluster& client, const dpp::slashcommand_t& event);

// Moderation commands
// ...
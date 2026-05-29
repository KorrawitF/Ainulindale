#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>

namespace config
{
    extern struct LobbyConfig {
        std::string lobby_secret;
    };

    bool load_config_file(LobbyConfig &lobbyConfig);
}

#endif
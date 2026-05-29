#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>

namespace config
{
    struct LobbyConfig {
        std::string lobby_secret;
    };

    struct AppConfig {
        uint64_t application_id;
        LobbyConfig lobbyConfig;
    };

    bool load_config_file(AppConfig &appConfig);
}

#endif
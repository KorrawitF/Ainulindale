#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>

namespace config
{
    extern struct AppConfig {
        uint64_t application_id;
        LobbyConfig lobbyConfig;
    };
    struct LobbyConfig {
        std::string lobby_secret;
    };

    bool load_config_file(AppConfig &appConfig);
}

#endif
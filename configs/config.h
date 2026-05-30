#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>

namespace config
{
    struct LobbyConfig {
        std::string lobby_secret;
    };

    struct ServerConfig {
        std::string host = "127.0.0.1";
        int port = 8080;
        std::string bot_token;      // Discord Bot token — server use only, never ship to clients
        std::string client_secret;  // Discord client secret — server use only
    };

    struct AppConfig {
        uint64_t application_id;
        LobbyConfig lobbyConfig;
        ServerConfig serverConfig;
    };

    bool load_config_file(AppConfig &appConfig);
}

#endif
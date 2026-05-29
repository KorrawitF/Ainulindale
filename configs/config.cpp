#include <iostream>
#include <cstdlib>
#include <config.h>

bool config::load_config_file(config::AppConfig &appConfig) {
    const char* secret = std::getenv("LOBBY_SECRET");
    if (!secret || secret[0] == '\0') {
        std::cout << "Lobby secret env not found" << std::endl;
        return false;
    }

    const char* app_id = std::getenv("APPLICATION_ID");
    if (!app_id || app_id[0] == '\0') {
        std::cout << "Application id env not found" << std::endl;
        return false;
    }

    appConfig.lobbyConfig.lobby_secret = secret;
    appConfig.application_id = strtoull(app_id, nullptr, 10);
    return true;
}

#include <iostream>
#include <cstdlib>
#include <config.h>

bool config::load_config_file(config::LobbyConfig &lobbyConfig) {
    const char* secret = std::getenv("LOBBY_SECRET");
    if (!secret || secret[0] == '\0') {
        std::cout << "Lobby secret env not found" << std::endl;
        return false;
    }

    lobbyConfig.lobby_secret = secret;
    return true;
}

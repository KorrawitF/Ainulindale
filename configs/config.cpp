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

    std::string Secret(secret);
    appConfig.lobbyConfig.lobby_secret = Secret;
    appConfig.application_id = strtoull(app_id, nullptr, 10);

    // Optional server-side fields — only needed when running AinulinServer
    const char* bot_token = std::getenv("BOT_TOKEN");
    if (bot_token && bot_token[0] != '\0') appConfig.serverConfig.bot_token = bot_token;

    const char* client_secret = std::getenv("CLIENT_SECRET");
    if (client_secret && client_secret[0] != '\0') appConfig.serverConfig.client_secret = client_secret;

    const char* server_host = std::getenv("SERVER_HOST");
    if (server_host && server_host[0] != '\0') appConfig.serverConfig.host = server_host;

    const char* server_port = std::getenv("SERVER_PORT");
    if (server_port && server_port[0] != '\0') appConfig.serverConfig.port = std::stoi(server_port);

    return true;
}

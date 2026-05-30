#include "session_manager.h"
#include "discord_token_service.h"
#include <config.h>

#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <thread>
#include <atomic>
#include <csignal>

std::atomic<bool> running = true;

void signalHandler(int) { running.store(false); }

void HandleClient(int client_sock, SessionManager& sessions, DiscordTokenService& discord) {
    char buf[256]{};
    int n = static_cast<int>(recv(client_sock, buf, sizeof(buf) - 1, 0));
    if (n <= 0) { close(client_sock); return; }

    std::istringstream ss(std::string(buf, static_cast<size_t>(n)));
    std::string command, player_id;
    ss >> command >> player_id;

    auto send_err = [&](const std::string& msg) {
        std::string err = "ERROR " + msg + "\n";
        send(client_sock, err.c_str(), err.size(), 0);
    };

    if (command != "SESSION" || player_id.empty()) {
        send_err("invalid request");
        close(client_sock);
        return;
    }

    std::string lobby_secret = sessions.GetOrCreateSession(player_id);
    std::string provisional_token = discord.GetProvisionalToken(player_id, player_id);

    if (provisional_token.empty()) {
        send_err("failed to get provisional token from Discord");
        close(client_sock);
        return;
    }

    std::string response = "OK " + lobby_secret + " " + provisional_token + "\n";
    send(client_sock, response.c_str(), response.size(), 0);
    std::cout << "✅ Session served — player: " << player_id
              << "  lobby: " << lobby_secret << "\n";
    close(client_sock);
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::cout << "🚀 Starting AinulinServer...\n";

    const char* app_id_env    = std::getenv("APPLICATION_ID");
    const char* bot_token_env = std::getenv("BOT_TOKEN");
    const char* port_env      = std::getenv("SERVER_PORT");

    if (!app_id_env || !bot_token_env) {
        std::cerr << "Required env vars: APPLICATION_ID, BOT_TOKEN\n";
        return 1;
    }

    config::AppConfig cfg;
    cfg.application_id           = strtoull(app_id_env, nullptr, 10);
    cfg.serverConfig.bot_token   = bot_token_env;
    cfg.serverConfig.port        = port_env ? std::stoi(port_env) : 8080;

    SessionManager sessions;
    DiscordTokenService discord(cfg.application_id, cfg.serverConfig.bot_token);

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Accept wakes up every second so the running flag is checked promptly.
    timeval tv{1, 0};
    setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(static_cast<uint16_t>(cfg.serverConfig.port));

    if (bind(server_sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "❌ bind failed on port " << cfg.serverConfig.port << "\n";
        return 1;
    }
    listen(server_sock, 16);
    std::cout << "🎮 Listening on port " << cfg.serverConfig.port << "\n";

    while (running) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        int client_sock = accept(server_sock, reinterpret_cast<sockaddr*>(&client_addr), &len);
        if (client_sock < 0) continue;  // timeout or signal — re-check running

        std::thread([client_sock, &sessions, &discord]() {
            HandleClient(client_sock, sessions, discord);
        }).detach();
    }

    close(server_sock);
    std::cout << "🛑 Server stopped.\n";
    return 0;
}

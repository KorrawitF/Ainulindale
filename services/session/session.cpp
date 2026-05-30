#include "session.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <cstring>

Session::Session(config::ServerConfig cfg) : serverCfg(std::move(cfg)) {}

void Session::FetchSession(const std::string& player_id, std::function<void(bool, SessionInfo)> callback) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "❌ Failed to create socket\n";
        callback(false, {});
        return;
    }

    struct hostent* host = gethostbyname(serverCfg.host.c_str());
    if (!host) {
        std::cerr << "❌ Cannot resolve game server host: " << serverCfg.host << "\n";
        close(sock);
        callback(false, {});
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(serverCfg.port));
    std::memcpy(&addr.sin_addr, host->h_addr_list[0], static_cast<size_t>(host->h_length));

    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "❌ Cannot connect to game server at "
                  << serverCfg.host << ":" << serverCfg.port << "\n";
        close(sock);
        callback(false, {});
        return;
    }

    std::string request = "SESSION " + player_id + "\n";
    send(sock, request.c_str(), request.size(), 0);

    char buf[512]{};
    int n = static_cast<int>(recv(sock, buf, sizeof(buf) - 1, 0));
    close(sock);

    if (n <= 0) {
        std::cerr << "❌ No response from game server\n";
        callback(false, {});
        return;
    }

    std::istringstream ss(std::string(buf, static_cast<size_t>(n)));
    std::string status, lobby_secret, provisional_token;
    ss >> status >> lobby_secret >> provisional_token;

    if (status != "OK" || lobby_secret.empty() || provisional_token.empty()) {
        std::cerr << "❌ Game server error: " << buf << "\n";
        callback(false, {});
        return;
    }

    std::cout << "✅ Session received from server (lobby: " << lobby_secret << ")\n";
    callback(true, {lobby_secret, provisional_token});
}

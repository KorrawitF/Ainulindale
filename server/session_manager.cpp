#include "session_manager.h"

#include <random>
#include <sstream>
#include <iomanip>

std::string SessionManager::GenerateSecret() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist;
    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(8) << dist(gen)
        << std::setw(8) << dist(gen)
        << std::setw(8) << dist(gen)
        << std::setw(8) << dist(gen);
    return oss.str();
}

std::string SessionManager::GetOrCreateSession(const std::string& player_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto [it, inserted] = sessions_.emplace(player_id, GameSession{});
    if (inserted) {
        it->second.lobby_secret = GenerateSecret();
    }
    return it->second.lobby_secret;
}

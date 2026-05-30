#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <unordered_map>
#include <mutex>

struct GameSession {
    std::string lobby_secret;
};

// Thread-safe manager that assigns one lobby_secret per player.
// Two players assigned the same session will get the same lobby_secret
// so they land in the same Discord lobby.
class SessionManager {
    std::unordered_map<std::string, GameSession> sessions_;
    std::mutex mutex_;

    static std::string GenerateSecret();

    public:
        // Returns the lobby_secret for player_id, creating a new session if needed.
        std::string GetOrCreateSession(const std::string& player_id);
};

#endif

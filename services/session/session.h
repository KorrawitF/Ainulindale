#ifndef SESSION_H
#define SESSION_H

#include <config.h>
#include <functional>
#include <string>

struct SessionInfo {
    std::string lobby_secret;
    std::string provisional_token;  // pass to ProvisionalAuth::Authenticate
};

// TCP client that requests a game session from AinulinServer.
// The server returns a lobby_secret (shared by all players in the match)
// and a provisional_token unique to this player.
class Session {
    config::ServerConfig serverCfg;

public:
    explicit Session(config::ServerConfig serverCfg);

    // Blocking call — connect to the game server and request a session.
    // Protocol: send "SESSION {player_id}\n", receive "OK {lobby_secret} {token}\n".
    void FetchSession(const std::string& player_id, std::function<void(bool, SessionInfo)> callback);
};

#endif

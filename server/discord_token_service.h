#ifndef DISCORD_TOKEN_SERVICE_H
#define DISCORD_TOKEN_SERVICE_H

#include <string>
#include <cstdint>

// Calls the Discord REST API to mint a bot-issued provisional access token
// for a given player. The returned token is passed to the game client, which
// uses it with ProvisionalAuth (GetProvisionalToken + DiscordBotIssuedAccessToken).
class DiscordTokenService {
    uint64_t application_id_;
    std::string bot_token_;

public:
    DiscordTokenService(uint64_t application_id, std::string bot_token);

    // Returns the access_token string, or empty string on failure.
    std::string GetProvisionalToken(const std::string& player_id, const std::string& display_name);
};

#endif

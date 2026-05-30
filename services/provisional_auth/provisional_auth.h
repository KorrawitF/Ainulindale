#ifndef PROVISIONAL_AUTH_H
#define PROVISIONAL_AUTH_H

#include "discordpp.h"
#include <config.h>
#include <functional>
#include <string>

// Alternative to Auth — no browser popup required.
// The caller must supply a bot-issued token obtained from the game server
// via the Session service. On success the client reaches Client::Status::Ready
// through the existing SetStatusChangedCallback.
class ProvisionalAuth {
    std::shared_ptr<discordpp::Client> client;
    config::AppConfig cfg;

public:
    ProvisionalAuth(std::shared_ptr<discordpp::Client>* client, config::AppConfig cfg);

    // Exchange a bot-issued token for a Discord provisional account token,
    // then call UpdateToken + Connect. onDone(true) fires after Connect() is called.
    void Authenticate(const std::string& botIssuedToken, std::function<void(bool)> onDone);
};

#endif

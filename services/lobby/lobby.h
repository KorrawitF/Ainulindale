#ifndef LOBBY_H
#define LOBBY_H

#include "discordpp.h"
#include <config.h>
#include <memory>
#include "voice.h"

class Lobby {
    private:
        std::shared_ptr<discordpp::Client> client;
        config::LobbyConfig cfg;
        std::shared_ptr<Voice> voice;

    public:
        Lobby(std::shared_ptr<discordpp::Client> *Client, config::LobbyConfig Cfg);
        void createOrJoint();
        void JoinWithSecret(const std::string& joinSecret);
        void LeaveLobby(uint64_t lobbyId);
};

#endif
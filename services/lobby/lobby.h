#ifndef LOBBY_H
#define LOBBY_H

#include "discordpp.h"
#include <config.h>

class Lobby {
    private:
        std::shared_ptr<discordpp::Client> client;
        config::LobbyConfig cfg;

    public:
        Lobby(std::shared_ptr<discordpp::Client> Client, config::LobbyConfig Cfg);
        void createOrJoint(std::shared_ptr<discordpp::Client> Client);
        void LeaveLobby(uint64_t lobbyId);
};

#endif
#include <iostream>
#include "discordpp.h"
#include "config.h"
#include "lobby.h"
#include "voice.h"


Lobby::Lobby(std::shared_ptr<discordpp::Client> *Client, config::LobbyConfig Cfg) : client(*Client), cfg(Cfg) {}

// Create or join a lobby from the client
void Lobby::createOrJoint() {
    client->CreateOrJoinLobby(cfg.lobby_secret, [this](discordpp::ClientResult result, uint64_t lobbyId) {
        if(result.Successful()) {
            std::cout << "🎮 Lobby created or joined successfully! Lobby Id: " << lobbyId << std::endl;
            voice = std::make_shared<Voice>(&client);
            voice->Call(lobbyId);
        } else {
            std::cerr << "❌ Lobby creation/join failed\n";
        }
    });
};

void Lobby::JoinWithSecret(const std::string& joinSecret) {
    client->CreateOrJoinLobby(joinSecret, [this](discordpp::ClientResult result, uint64_t lobbyId) {
        if (result.Successful()) {
            std::cout << "🎮 Joined lobby from invite! Lobby Id: " << lobbyId << std::endl;
            voice = std::make_shared<Voice>(&client);
            voice->Call(lobbyId);
        } else {
            std::cerr << "❌ Failed to join lobby from invite\n";
        }
    });
}

void Lobby::LeaveLobby(uint64_t lobbyId) {
    client->LeaveLobby(lobbyId, [&](discordpp::ClientResult result) {
        if(result.Successful()) {
            std::cout << "🎮 Left lobby successfully! Lobby Id: " << lobbyId << std::endl;
        } else {
            std::cerr << "❌ Leaving lobby failed\n";
        }
    });
};
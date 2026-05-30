#define DISCORDPP_IMPLEMENTATION
#include "discordpp.h"
#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <functional>
#include <csignal>
#include <iostream>
#include <config.h>
#include <auth.h>
#include <lobby.h>
#include <activity.h>
#include <friend.h>

// Create a flag to stop the application
std::atomic<bool> running = true;

// Signal handler to stop the application
void signalHandler(int signum) {
  running.store(false);
}

int main() {
  std::signal(SIGINT, signalHandler);
  std::cout << "🚀 Initializing Discord SDK...\n";

  config::AppConfig cfg;
  if (!config::load_config_file(cfg)) {
    std::cerr << "Failed to load config from env" << std::endl;
    std::abort();
  }

  // Create our Discord Client
  auto client = std::make_shared<discordpp::Client>();

  // Define activity instance
  discordpp::Activity activity;
  discordpp::ActivityParty party;
  discordpp::ActivitySecrets secrets;
  Activity act(&activity, &party, &secrets);
  act.SetJoinSecret(cfg.lobbyConfig.lobby_secret);  // friends who accept the invite join this lobby

  Friend friendService(&client);

  Auth auth(&client, cfg);
  auth.Authorize();

  client->AddLogCallback([](auto message, auto severity) {
    std::cout << "[" << EnumToString(severity) << "] " << message << std::endl;
  }, discordpp::LoggingSeverity::Info);

  auto lobby = std::make_shared<Lobby>(&client, cfg.lobbyConfig);

  client->SetStatusChangedCallback([client, lobby, act, friendService](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
    std::cout << "🔄 Status changed: " << discordpp::Client::StatusToString(status) << std::endl;

    if (status == discordpp::Client::Status::Ready) {
      std::cout << "✅ Client is ready! You can now call SDK functions.\n";
      std::cout << "👥 Friends Count: " << client->GetRelationships().size() << std::endl;
      
      lobby->createOrJoint();

      client->UpdateRichPresence(act.GetActivity(), [friendService](discordpp::ClientResult result) {
        if (!result.Successful()) return;
        std::cout << "✅ Rich presence updated!\n";
        friendService.SendInvite(689867889305452755, "TESTING");
      });
      
    } else if (error != discordpp::Client::Error::None) {
      std::cerr << "❌ Connection Error: " << discordpp::Client::ErrorToString(error) << " - Details: " << errorDetail << std::endl;
    }
  });

  // Recipient clicks "Join" in Discord while the game is already running,
  // OR Discord launches this binary via RegisterLaunchCommand and passes the secret.
  client->SetActivityJoinCallback([lobby](std::string joinSecret) {
    std::cout << "🎮 Join triggered from Discord — joining lobby...\n";
    lobby->JoinWithSecret(joinSecret);
  });

  // When this client receives an activity invite, auto-accept and join the lobby.
  client->SetActivityInviteCreatedCallback([client, lobby](discordpp::ActivityInvite invite) {
    std::cout << "📨 Activity invite received! Accepting...\n";
    client->AcceptActivityInvite(invite, [lobby](discordpp::ClientResult result, std::string joinSecret) {
      if (!result.Successful()) {
        std::cerr << "❌ Failed to accept invite: " << result.Error() << std::endl;
        return;
      }
      std::cout << "✅ Invite accepted! Joining lobby...\n";
      lobby->JoinWithSecret(joinSecret);
    });
  });

  // Keep application running to allow SDK to receive events and callbacks
  while (running) {
    discordpp::RunCallbacks();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
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


// Create a flag to stop the application
std::atomic<bool> running = true;

// Signal handler to stop the application
void signalHandler(int signum) {
  running.store(false);
}

int main() {
  std::signal(SIGINT, signalHandler);
  std::cout << "🚀 Initializing Discord SDK...\n";

  auto cfg = config::AppConfig{};
  if (!config::load_config_file(cfg)) {
    std::cerr << "Failed to load config from env" << std::endl;
    std::abort();
  }

  // Create our Discord Client
  auto client = std::make_shared<discordpp::Client>();

  Auth auth(&client, cfg);
  auth.Authorize();

  client->AddLogCallback([](auto message, auto severity) {
    std::cout << "[" << EnumToString(severity) << "] " << message << std::endl;
  }, discordpp::LoggingSeverity::Info);

  client->SetStatusChangedCallback([client](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
    std::cout << "🔄 Status changed: " << discordpp::Client::StatusToString(status) << std::endl;

    if (status == discordpp::Client::Status::Ready) {
      std::cout << "✅ Client is ready! You can now call SDK functions.\n";
      std::cout << "👥 Friends Count: " << client->GetRelationships().size() << std::endl;
    } else if (error != discordpp::Client::Error::None) {
      std::cerr << "❌ Connection Error: " << discordpp::Client::ErrorToString(error) << " - Details: " << errorDetail << std::endl;
    }
  });

  // Keep application running to allow SDK to receive events and callbacks
  while (running) {
    discordpp::RunCallbacks();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  return 0;
}
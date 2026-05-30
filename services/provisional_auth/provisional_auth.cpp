#include "provisional_auth.h"
#include <iostream>

ProvisionalAuth::ProvisionalAuth(std::shared_ptr<discordpp::Client>* Client, config::AppConfig Cfg)
    : client(*Client), cfg(Cfg) {}

void ProvisionalAuth::Authenticate(const std::string& botIssuedToken, std::function<void(bool)> onDone) {
    std::cout << "🔐 Authenticating with bot-issued token...\n";

    client->GetProvisionalToken(
        cfg.application_id,
        discordpp::AuthenticationExternalAuthType::DiscordBotIssuedAccessToken,
        botIssuedToken,
        [this, onDone](discordpp::ClientResult result,
                       std::string accessToken,
                       std::string /*refreshToken*/,
                       discordpp::AuthorizationTokenType /*tokenType*/,
                       int32_t expiresIn,
                       std::string /*scope*/) {
            if (!result.Successful()) {
                std::cerr << "❌ Provisional auth failed: " << result.Error() << std::endl;
                onDone(false);
                return;
            }
            std::cout << "✅ Provisional token received (expires in " << expiresIn << "s)\n";

            client->UpdateToken(
                discordpp::AuthorizationTokenType::Bearer,
                accessToken,
                [this, onDone](discordpp::ClientResult res) {
                    if (!res.Successful()) {
                        std::cerr << "❌ UpdateToken failed\n";
                        onDone(false);
                        return;
                    }
                    std::cout << "🔑 Token updated, connecting to Discord...\n";
                    client->Connect();
                    onDone(true);
                });
        });
}

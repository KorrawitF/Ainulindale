#include <auth.h>
#include "discordpp.h"
#include <iostream>

Auth::Auth(std::shared_ptr<discordpp::Client> Client, config::AppConfig Cfg) : client(Client), cfg(Cfg), codeVerifier(client->CreateAuthorizationCodeVerifier()) {
    args.SetClientId(cfg.application_id);
    args.SetScopes(discordpp::Client::GetDefaultPresenceScopes());
    args.SetCodeChallenge(codeVerifier.Challenge());
}

void Auth::Authorize() {
    client->Authorize(args, [*this](discordpp::ClientResult result, std::string code, std::string redirectUri) {
        if (!result.Successful()) {
            std::cerr << "❌ Authentication Error: " << result.Error() << std::endl;
            return;
        } else {
            std::cout << "✅ Authorization successful! Getting access token...\n";

            // Exchange auth code for access token
            client->GetToken(cfg.application_id, code, codeVerifier.Verifier(), redirectUri,
                [*this](discordpp::ClientResult result,
                std::string accessToken,
                std::string refreshToken,
                discordpp::AuthorizationTokenType tokenType,
                int32_t expiresIn,
                std::string scope) {
                // Next Step: Update the token and connect
                client->UpdateToken(discordpp::AuthorizationTokenType::Bearer,  accessToken, [*this](discordpp::ClientResult result) {
                    if(result.Successful()) {
                    std::cout << "🔑 Token updated, connecting to Discord...\n";
                    client->Connect();
                    }
                });
                std::cout << "🔓 Access token received! Establishing connection...\n";
                // Next Step: Update the token and connect
            });
        }
    });
}

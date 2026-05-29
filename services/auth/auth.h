#ifndef AUTH_H
#define AUTH_H

#include "discordpp.h"
#include <config.h>

class Auth {
    private:
        std::shared_ptr<discordpp::Client> client;
        config::AppConfig cfg;
        discordpp::AuthorizationCodeVerifier codeVerifier = client->CreateAuthorizationCodeVerifier();
    
    public:
        Auth(std::shared_ptr<discordpp::Client> Client, config::AppConfig Cfg);
        void Authorize(discordpp::AuthorizationArgs args);
};


#endif
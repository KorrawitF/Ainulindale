#ifndef AUTH_H
#define AUTH_H

#include "discordpp.h"
#include <config.h>

class Auth {
    private:
        std::shared_ptr<discordpp::Client> client;
        config::AppConfig cfg;
        discordpp::AuthorizationArgs args;
        discordpp::AuthorizationCodeVerifier codeVerifier;
    
    public:
        Auth(std::shared_ptr<discordpp::Client> *Client, config::AppConfig Cfg);
        void Authorize();
};


#endif
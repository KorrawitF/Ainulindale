#ifndef AUTH_H
#define AUTH_H

#include "discordpp.h"

class Auth {
    private:
        std::shared_ptr<discordpp::Client> client;
    
    public:
        Auth(std::shared_ptr<discordpp::Client> Client);
};


#endif
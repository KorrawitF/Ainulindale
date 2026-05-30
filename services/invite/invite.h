#ifndef INVITE_H
#define INVITE_H

#include "discordpp.h"

class Invite {
    private:
        std::shared_ptr<discordpp::Client> client;
    public:
        Invite(std::shared_ptr<discordpp::Client> *Client);

};

#endif
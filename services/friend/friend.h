#ifndef FRIEND_H
#define FRIEND_H

#include <string>
#include "discordpp.h"

class Friend {
    private:
        std::shared_ptr<discordpp::Client> client;
    public:
        Friend(std::shared_ptr<discordpp::Client> *Client);
        bool SendInvite(uint64_t userId, const std::string &message);

};

#endif
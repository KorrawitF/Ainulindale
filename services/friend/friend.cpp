#include "friend.h"
#include "discordpp.h"
#include <string>
#include <iostream>

Friend::Friend(std::shared_ptr<discordpp::Client> *Client) : client(*Client) {}

bool Friend::SendInvite(uint64_t userId, const std::string &message) const {
    bool ret;
    client->SendActivityInvite(userId, message, [&ret](discordpp::ClientResult result) {
        if (result.Successful()) {
            std::cout << "Invitation sent successfully." << std::endl;
            ret = true;
        } else {
            std::cout << "Failed to send invitation, err: " + result.Error() << std::endl;
            ret = false;
        } 
    });
    return ret;
}
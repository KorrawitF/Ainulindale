#include "friend.h"
#include "discordpp.h"
#include <string>

Friend::Friend(std::shared_ptr<discordpp::Client> *Client) : client(*Client) {}

bool Friend::SendInvite(uint64_t userId, const std::string &message) {
    bool ret;
    client->SendActivityInvite(userId, message, [&ret](discordpp::ClientResult result) {
        if (result.Successful()) {
            ret = true;
        } else {
            ret = false;
        } 
    });
    return ret;
}
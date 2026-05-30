#include "friend.h"
#include "discordpp.h"
#include <string>
#include <iostream>

Friend::Friend(std::shared_ptr<discordpp::Client> *Client) : client(*Client) {}

void Friend::SendInvite(uint64_t userId, const std::string &message) const {
    client->SendActivityInvite(userId, message, [](discordpp::ClientResult result) {
        if (result.Successful()) {
            std::cout << "✅ Invitation sent successfully." << std::endl;
        } else {
            std::cerr << "❌ Failed to send invitation: " << result.Error() << std::endl;
        }
    });
}
#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "discordpp.h"

class Activity {
    private:
        discordpp::Activity activity;
        discordpp::ActivityParty party;
        discordpp::ActivitySecrets secrets;
    public:
        Activity(discordpp::Activity *Activity, discordpp::ActivityParty *Party, discordpp::ActivitySecrets *Secrets);
        discordpp::Activity GetActivity() const;
};

#endif
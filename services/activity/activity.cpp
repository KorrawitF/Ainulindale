#include <activity.h>
#include "discordpp.h"

Activity::Activity(discordpp::Activity *Activity, discordpp::ActivityParty *Party, discordpp::ActivitySecrets *Secrets) : activity(*Activity), party(*Party), secrets(*Secrets) {
    activity.SetType(discordpp::ActivityTypes::Playing);
    activity.SetDetails("Spatial Voice");
    activity.SetState("In Development");

    party.SetId("party1234");
    party.SetCurrentSize(1);          
    party.SetMaxSize(5);  
    activity.SetParty(party);

    activity.SetSecrets(secrets);

    using P = discordpp::ActivityGamePlatforms;
    activity.SetSupportedPlatforms(static_cast<P>(
        static_cast<int>(P::Desktop)  |
        static_cast<int>(P::IOS)      |
        static_cast<int>(P::Android)  |
        static_cast<int>(P::Embedded) |   // covers web/browser clients
        static_cast<int>(P::Samsung)
    ));
}

void Activity::SetJoinSecret(const std::string& secret) {
    secrets.SetJoin(secret);
    activity.SetSecrets(secrets);
}

discordpp::Activity Activity::GetActivity() const
{
    return activity;
};
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

    secrets.SetJoin("joinsecret1234"); 
    activity.SetSecrets(secrets);

    activity.SetSupportedPlatforms(discordpp::ActivityGamePlatforms::Desktop);
}

discordpp::Activity Activity::GetActivity() const
{
    return activity;
};
#ifndef VOICE_H
#define VOICE_H

#include "discordpp.h"

class Voice {
    private:
        std::shared_ptr<discordpp::Client> client;

    public:
        Voice(std::shared_ptr<discordpp::Client> client);

};

#endif
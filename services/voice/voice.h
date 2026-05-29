#ifndef VOICE_H
#define VOICE_H

#include "discordpp.h"
#include <optional>

void ReceivedCb(uint64_t userId, int16_t *data, uint64_t samplesPerChannel, int32_t sampleRate, uint64_t channels, bool &outShouldMute);
void CapturedCb(int16_t *data, uint64_t samplesPerChannel, int32_t sampleRate, uint64_t channels);

class Voice {
    private:
        std::shared_ptr<discordpp::Client> client;
        std::optional<discordpp::Call> call;
    public:
        Voice(std::shared_ptr<discordpp::Client> *client);
        void Call(uint64_t lobbyId);
};

#endif
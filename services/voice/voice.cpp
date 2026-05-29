#include <voice.h>
#include "discordpp.h"
#include <iostream>

Voice::Voice(std::shared_ptr<discordpp::Client> *Client) : client(*Client) {}

void Voice::Call(uint64_t lobbyId) {
    client->StartCallWithAudioCallbacks(
        lobbyId, 
        ReceivedCb,
        CapturedCb
    );
};

void ReceivedCb(uint64_t userId, int16_t *data, const size_t samplesPerChannel, int sampleRate, const size_t channels, bool &outShouldMuteData) {
    std::string result = "Received voice from userId: ";
    result += userId;
    std::cout << result << std::endl;
    for(int i = 0; i < samplesPerChannel * channels; i++) {
        data[i] *= 0.5;
    }
};

void CapturedCb(int16_t *data, uint64_t samplesPerChannel, int32_t sampleRate, uint64_t channels) {};
#include <voice.h>
#include "discordpp.h"
#include <iostream>

Voice::Voice(std::shared_ptr<discordpp::Client> *Client) : client(*Client) {}

void Voice::Call(uint64_t lobbyId) {
    std::string log = "Starting call to lobby: ";
    log += std::to_string(lobbyId);
    
    std::cout << log << std::endl;
    call = client->StartCallWithAudioCallbacks(
        lobbyId,
        ReceivedCb,
        CapturedCb
    );

    if (!call) {
        std::cerr << "⚠️  StartCall returned null (already in this voice channel?)" << std::endl;
        return;
    }

};

void ReceivedCb(uint64_t userId, int16_t *data, uint64_t samplesPerChannel, int32_t sampleRate, uint64_t channels, bool &outShouldMute) {
    std::string log = "Received voice from userId: ";
    log += std::to_string(userId);
    std::cout << log << std::endl;

    for(uint64_t i = 0; i < samplesPerChannel * channels; i++) {
        data[i] = static_cast<int16_t>(data[i] * 0.5);
    }
};

void CapturedCb(int16_t *data, uint64_t samplesPerChannel, int32_t sampleRate, uint64_t channels) {};
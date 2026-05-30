#include "discord_token_service.h"

#include <curl/curl.h>
#include <iostream>

namespace {
    size_t WriteCallback(char* ptr, size_t size, size_t nmemb, std::string* out) {
        out->append(ptr, size * nmemb);
        return size * nmemb;
    }

    // Minimal JSON string extraction without pulling in a JSON library.
    std::string ExtractJsonString(const std::string& json, const std::string& key) {
        const std::string search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) return {};
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return {};
        return json.substr(pos, end - pos);
    }
}

DiscordTokenService::DiscordTokenService(uint64_t application_id, std::string bot_token)
    : application_id_(application_id), bot_token_(std::move(bot_token)) {}

std::string DiscordTokenService::GetProvisionalToken(const std::string& player_id,
                                                      const std::string& display_name) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "❌ curl_easy_init failed\n";
        return {};
    }

    // TODO: Confirm the exact endpoint from Discord's Social SDK server-side docs.
    // Discord Social SDK reference: https://discord.com/developers/docs/social-sdk
    // Expected pattern for bot-issued provisional tokens:
    //   POST /api/v10/applications/{application_id}/provisional-tokens
    std::string url = "https://discord.com/api/v10/applications/"
                    + std::to_string(application_id_)
                    + "/provisional-tokens";

    std::string body = "{\"external_user_id\":\"" + player_id
                     + "\",\"display_name\":\"" + display_name + "\"}";

    std::string auth_header = "Authorization: Bot " + bot_token_;
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, auth_header.c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "❌ Discord REST call failed: " << curl_easy_strerror(res) << "\n";
        return {};
    }

    std::string token = ExtractJsonString(response, "access_token");
    if (token.empty()) {
        std::cerr << "❌ Unexpected Discord response: " << response << "\n";
    }
    return token;
}

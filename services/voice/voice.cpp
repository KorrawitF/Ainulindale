#include <voice.h>
#include "discordpp.h"

Voice::Voice(std::shared_ptr<discordpp::Client> *Client) : client(*Client) {};
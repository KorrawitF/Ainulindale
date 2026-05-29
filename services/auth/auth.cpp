#include <auth.h>
#include "discordpp.h"
#include <iostream>

Auth::Auth(std::shared_ptr<discordpp::Client> Client) : client(Client) {};

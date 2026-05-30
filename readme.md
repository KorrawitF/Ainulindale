# Ainulindale

A C++ console application integrating Discord Social SDK features: proximity voice chat, lobbies, rich presence, and friend invites.

## Prerequisites

- C++20 compiler (Clang, GCC, or MSVC)
- CMake 3.10+
- Discord Social SDK binaries (included in `lib/discord_social_sdk/`)
- A Discord application with the Social SDK enabled ([Discord Developer Portal](https://discord.com/developers/applications))

## Setup

1. In the Discord Developer Portal, create an application and enable the Social SDK.
2. Under **OAuth2**, add `http://127.0.0.1/callback` as a redirect URI and enable **Public Client**.
3. Export your credentials:

```bash
export APPLICATION_ID=<your_discord_app_id>
export LOBBY_SECRET=<your_lobby_secret>
```

## Build & Run

```bash
# Build
make compile

# Run
./build/Ainulindale
```

On macOS, if you get a security warning about `libdiscord_partner_sdk.dylib`, go to **System Settings → Privacy & Security** and click **Open Anyway**.

## Architecture

The app is single-threaded and event-driven. `main.cpp` runs a loop calling `discordpp::RunCallbacks()` every 10ms — all SDK callbacks are dispatched here.

**Initialization sequence:**
1. Load `APPLICATION_ID` and `LOBBY_SECRET` from environment (`configs/config.cpp`)
2. Create `discordpp::Client`
3. Start OAuth2 PKCE flow (`services/auth/`) — opens browser for Discord login
4. On `Client::Status::Ready`: update rich presence, create/join lobby
5. On lobby join: start proximity voice call with audio callbacks

**Services (`services/`):**

| Service | What it does |
|---------|-------------|
| `auth/` | OAuth2 PKCE: `Authorize` → `GetToken` → `UpdateToken` → `Connect` |
| `activity/` | Configures rich presence (type, state, party, join secret) |
| `lobby/` | Creates or joins a lobby via `LOBBY_SECRET`; owns the `Voice` instance |
| `voice/` | Starts the call; `ReceivedCb` applies 0.5 gain to incoming audio |
| `friend/` | Sends activity invites via `SendActivityInvite` |

Each service takes a `std::shared_ptr<discordpp::Client>` and wraps async SDK calls with lambda callbacks.

## Runtime Dependencies

The build copies the SDK shared library to `build/` automatically. Ensure it stays alongside the executable:

| Platform | Library |
|----------|---------|
| macOS | `libdiscord_partner_sdk.dylib` |
| Linux | `libdiscord_partner_sdk.so` |
| Windows | `discord_partner_sdk.dll` |

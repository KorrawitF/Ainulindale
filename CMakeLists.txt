cmake_minimum_required(VERSION 3.10)
project(Ainulindale)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ---------------------------------------------------------------------------
# Game client — Ainulindale
# ---------------------------------------------------------------------------
add_executable(Ainulindale
    main.cpp
    configs/config.cpp
    services/auth/auth.cpp
    services/lobby/lobby.cpp
    services/voice/voice.cpp
    services/friend/friend.cpp
    services/activity/activity.cpp
    services/provisional_auth/provisional_auth.cpp
    services/session/session.cpp
)

# Define some handy Social SDK variables
set(DISCORD_SDK_ROOT "${CMAKE_SOURCE_DIR}/lib/discord_social_sdk")
set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_ROOT}/lib/release")
set(DISCORD_SDK_BIN_DIR "${DISCORD_SDK_ROOT}/bin/release")
set(DISCORD_SDK_INCLUDE_DIR "${DISCORD_SDK_ROOT}/include")

# Include for Social SDK headers and local project headers
target_include_directories(Ainulindale PRIVATE
    ${DISCORD_SDK_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/configs
    ${CMAKE_SOURCE_DIR}/services/auth
    ${CMAKE_SOURCE_DIR}/services/lobby
    ${CMAKE_SOURCE_DIR}/services/voice
    ${CMAKE_SOURCE_DIR}/services/friend
    ${CMAKE_SOURCE_DIR}/services/activity
    ${CMAKE_SOURCE_DIR}/services/provisional_auth
    ${CMAKE_SOURCE_DIR}/services/session
)

# Platform-specific Social SDK library paths
if(WIN32)
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/discord_partner_sdk.lib")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll")
elseif(APPLE)
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
else() # Linux
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
endif()

# Link the Social SDK library
target_link_libraries(Ainulindale PRIVATE ${DISCORD_LIB_PATH})

# Set the runtime search path (RPATH) for Linux and macOS
if(UNIX)
    # Use RPATH when building
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    # Set the RPATH to use the lib directory relative to the executable
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    if(APPLE)
        set(CMAKE_INSTALL_RPATH "@executable_path")
    endif()
endif()

# Copy Social SDK shared library to output directory, so it's available at runtime.
add_custom_command(TARGET Ainulindale POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${DISCORD_SHARED_LIB}"
    $<TARGET_FILE_DIR:Ainulindale>
)

# ---------------------------------------------------------------------------
# Game server — AinulinServer (requires libcurl for Discord REST calls)
# ---------------------------------------------------------------------------
find_package(CURL)
if(CURL_FOUND)
    add_executable(AinulinServer
        server/main.cpp
        server/session_manager.cpp
        server/discord_token_service.cpp
        configs/config.cpp
    )
    target_include_directories(AinulinServer PRIVATE
        ${CMAKE_SOURCE_DIR}/configs
        ${CMAKE_SOURCE_DIR}/server
    )
    target_link_libraries(AinulinServer PRIVATE CURL::libcurl)
    message(STATUS "curl found — AinulinServer will be built")
else()
    message(STATUS "curl not found — skipping AinulinServer (install libcurl to enable)")
endif()

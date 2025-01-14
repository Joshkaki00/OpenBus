#pragma once

#include "AudioEngine.h"
#include <zmq.hpp>
#include <nlohmann/json.hpp>

class ZeroMQServer
{
public:
    ZeroMQServer(AudioEngine& audioEngine);
    ~ZeroMQServer();

    void listen();

private:
    nlohmann::json processCommand(const nlohmann::json& command);

    zmq::context_t context;
    zmq::socket_t socket;
    AudioEngine& audioEngine;
};

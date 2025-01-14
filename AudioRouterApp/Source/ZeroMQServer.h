#pragma once

#include <nlohmann/json.hpp>
#include <zmq.hpp>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    ZeroMQServer(AudioEngine& engine);
    void listen();

private:
    AudioEngine& audioEngine;
    zmq::context_t context;
    zmq::socket_t socket;
};

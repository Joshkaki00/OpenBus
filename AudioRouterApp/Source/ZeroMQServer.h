#pragma once

#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include "AudioEngine.h"

using json = nlohmann::json;

class ZeroMQServer
{
public:
    ZeroMQServer(AudioEngine& engine);
    void listen();

private:
    AudioEngine& audioEngine;
    zmq::context_t context;
    zmq::socket_t socket;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ZeroMQServer)
};

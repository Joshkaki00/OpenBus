#pragma once

#include <nlohmann/json.hpp>
#include <zmq.hpp>
#include <string>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    ZeroMQServer();
    ~ZeroMQServer();

    void listen();

private:
    nlohmann::json processCommand(const nlohmann::json& command, AudioEngine& audioEngine);

    zmq::context_t context{1};
    zmq::socket_t socket;
};

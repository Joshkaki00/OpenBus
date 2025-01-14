#pragma once

#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    ZeroMQServer();
    ~ZeroMQServer();

    void listen();

private:
    nlohmann::json processCommand(const nlohmann::json& command);

    zmq::context_t context{1};
    zmq::socket_t socket{context, ZMQ_REP};
    AudioEngine audioEngine;
};

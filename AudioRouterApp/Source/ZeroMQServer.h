#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include "AudioEngine.h"
#include <nlohmann/json.hpp>
#include <zmq.hpp>

using json = nlohmann::json;

class ZeroMQServer {
public:
    ZeroMQServer(AudioEngine& engine);
    void listen();

private:
    json processCommand(const json& command); // Declare processCommand

    AudioEngine& audioEngine;
    zmq::context_t context;
    zmq::socket_t socket;
};

#endif // ZEROMQSERVER_H

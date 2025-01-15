#ifndef ZEROMQ_SERVER_H
#define ZEROMQ_SERVER_H

#include <nlohmann/json.hpp>
#include <zmq.hpp>
#include <string>
#include "AudioEngine.h"

class ZeroMQServer {
public:
    ZeroMQServer();
    ~ZeroMQServer();

    void listen();

private:
    nlohmann::json processCommand(const nlohmann::json& command, AudioEngine& audioEngine);

    zmq::context_t context{1};
    zmq::socket_t socket;
    AudioEngine audioEngine; // Ensure AudioEngine has a public default constructor
};

#endif // ZEROMQ_SERVER_H

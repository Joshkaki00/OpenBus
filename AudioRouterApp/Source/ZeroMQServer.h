#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include <zmq.hpp>
#include "AudioEngine.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ZeroMQServer {
public:
    ZeroMQServer(AudioEngine& engine);
    ~ZeroMQServer();

    void listen(); // Listen for incoming commands from the PyQt5 frontend

private:
    AudioEngine& audioEngine;      // Reference to the audio engine
    zmq::context_t context;        // ZeroMQ context
    zmq::socket_t socket;          // ZeroMQ socket for communication
};

#endif // ZEROMQSERVER_H

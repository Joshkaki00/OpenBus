#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include <zmq.hpp>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    ZeroMQServer(AudioEngine& engine); // Constructor
    ~ZeroMQServer();                   // Destructor

    void listen();                     // Main listener loop

private:
    zmq::context_t context;            // ZMQ context
    zmq::socket_t socket;              // ZMQ socket
    AudioEngine& audioEngine;          // Reference to AudioEngine
};

#endif // ZEROMQSERVER_H

#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include <zmq.hpp>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    ZeroMQServer(AudioEngine& engine);
    void listen();

private:
    zmq::context_t context;
    zmq::socket_t socket;
    AudioEngine& audioEngine;
};

#endif // ZEROMQSERVER_H

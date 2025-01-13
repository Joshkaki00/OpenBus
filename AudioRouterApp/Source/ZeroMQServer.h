#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include <zmq.hpp>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    explicit ZeroMQServer(AudioEngine& engine);
    ~ZeroMQServer();

    void listen();

private:
    AudioEngine& audioEngine;
    zmq::context_t context;
    zmq::socket_t socket;
};

#endif // ZEROMQSERVER_H

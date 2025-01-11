#include "ZeroMQServer.h"
#include <iostream>

ZeroMQServer::ZeroMQServer(AudioEngine& engine) : audioEngine(engine)
{
    try
    {
        context = zmq::context_t(1);
        socket = zmq::socket_t(context, zmq::socket_type::rep);
        socket.bind("tcp://*:5555");
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "ZeroMQ initialization error: " << e.what() << std::endl;
    }
}


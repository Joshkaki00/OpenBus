#include "ZeroMQServer.h"

ZeroMQServer::ZeroMQServer(AudioEngine& engine) : audioEngine(engine)
{
    context = zmq::context_t(1);
    socket = zmq::socket_t(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");
}

void ZeroMQServer::listen()
{
    while (true)
    {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);

        std::string msg(static_cast<char*>(request.data()), request.size());
        audioEngine.handleCommand(msg);

        zmq::message_t reply(5);
        memcpy(reply.data(), "Done", 5);
        socket.send(reply, zmq::send_flags::none);
    }
}

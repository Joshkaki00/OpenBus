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

void ZeroMQServer::listen()
{
    zmq::pollitem_t items[] = { { static_cast<void*>(socket), 0, ZMQ_POLLIN, 0 } };

    while (true)
    {
        try
        {
            zmq::poll(&items[0], 1, std::chrono::milliseconds(10)); // Poll every 10 ms

            if (items[0].revents & ZMQ_POLLIN)
            {
                zmq::message_t request;
                auto recvResult = socket.recv(request, zmq::recv_flags::none);
                if (!recvResult)
                {
                    std::cerr << "ZeroMQ receive error!" << std::endl;
                    continue; // Retry instead of exiting
                }

                // Handle received message
                std::string msg(static_cast<char*>(request.data()), request.size());
                std::cerr << "Received command: " << msg << std::endl; // Debugging log
                audioEngine.handleCommand(msg);

                // Send reply
                zmq::message_t reply(5);
                memcpy(reply.data(), "Done", 5);
                socket.send(reply, zmq::send_flags::none);
            }
        }
        catch (const zmq::error_t& e)
        {
            std::cerr << "ZeroMQ server error: " << e.what() << std::endl;
        }
    }
}

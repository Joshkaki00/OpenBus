#include "ZeroMQServer.h"
#include <iostream>

// Constructor
ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine), // Ensure the initialization order matches declaration order
      context(1),          // Initialize the context first
      socket(context, zmq::socket_type::rep) // Initialize the socket after the context
{
    try
    {
        socket.bind("tcp://*:5555"); // Bind the socket to TCP port 5555
        std::cout << "ZeroMQ server started on port 5555" << std::endl;
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "ZeroMQ initialization error: " << e.what() << std::endl;
        throw; // Rethrow the exception to handle it at a higher level
    }
}

// Destructor
ZeroMQServer::~ZeroMQServer()
{
    try
    {
        socket.close();
        context.close();
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "ZeroMQ cleanup error: " << e.what() << std::endl;
    }
}

// Main listener loop
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

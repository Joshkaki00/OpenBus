#include "ZeroMQServer.h"
#include <iostream>

// Constructor
ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine),
      context(1), // Create a single-threaded context
      socket(context, zmq::socket_type::rep) // Create a REP (reply) socket
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
            zmq::poll(&items[0], 1, std::chrono::milliseconds(10)); // Poll with a 10 ms timeout

            if (items[0].revents & ZMQ_POLLIN)
            {
                zmq::message_t request;
                auto recvResult = socket.recv(request, zmq::recv_flags::none);
                if (!recvResult)
                {
                    std::cerr << "Failed to receive message from client" << std::endl;
                    continue; // Retry on receive failure
                }

                std::string msg(static_cast<char*>(request.data()), request.size());
                std::cout << "Received command: " << msg << std::endl;

                // Handle the command
                handleCommand(msg);
            }
        }
        catch (const zmq::error_t& e)
        {
            std::cerr << "ZeroMQ server error: " << e.what() << std::endl;
        }
    }
}

// Handle incoming commands
void ZeroMQServer::handleCommand(const std::string& msg)
{
    try
    {
        audioEngine.handleCommand(msg); // Forward the command to the audio engine
        sendResponse("Command executed successfully");
    }
    catch (const std::exception& e)
    {
        std::cerr << "Command handling error: " << e.what() << std::endl;
        sendResponse(std::string("Error: ") + e.what());
    }
}

// Send response to the client
void ZeroMQServer::sendResponse(const std::string& response)
{
    try
    {
        zmq::message_t reply(response.size());
        memcpy(reply.data(), response.c_str(), response.size());
        socket.send(reply, zmq::send_flags::none);
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "Failed to send response: " << e.what() << std::endl;
    }
}

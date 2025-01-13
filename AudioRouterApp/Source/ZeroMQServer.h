#ifndef ZEROMQSERVER_H
#define ZEROMQSERVER_H

#include <zmq.hpp>
#include "AudioEngine.h"

class ZeroMQServer
{
public:
    explicit ZeroMQServer(AudioEngine& engine);
    ~ZeroMQServer();

    void listen(); // Starts the server to handle incoming messages

private:
    zmq::context_t context; // ZeroMQ context
    zmq::socket_t socket;   // ZeroMQ socket for communication
    AudioEngine& audioEngine; // Reference to the audio engine

    void handleCommand(const std::string& msg); // Handles received commands
    void sendResponse(const std::string& response); // Sends a response to the client
};

#endif // ZEROMQSERVER_H

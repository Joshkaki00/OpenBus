#include "ZeroMQServer.h"
#include <iostream>

ZeroMQServer::ZeroMQServer() : socket(context, ZMQ_REP)
{
    socket.bind("tcp://*:5555");
}

ZeroMQServer::~ZeroMQServer()
{
    socket.close();
    context.close();
}

void ZeroMQServer::listen()
{
    while (true)
    {
        zmq::message_t request;
        auto result = socket.recv(request, zmq::recv_flags::none);
        if (!result)
        {
            std::cerr << "Failed to receive message from socket." << std::endl;
            return; // Or handle the error appropriately
        }

        try
        {
            std::string msg(static_cast<char*>(request.data()), request.size());
            nlohmann::json command = nlohmann::json::parse(msg);

            // Use the Singleton instance of AudioEngine
            nlohmann::json response = processCommand(command, AudioEngine::getInstance());

            zmq::message_t reply(response.dump());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const std::exception& e)
        {
            nlohmann::json errorResponse = {
                {"status", "error"},
                {"message", e.what()}
            };

            zmq::message_t reply(errorResponse.dump());
            socket.send(reply, zmq::send_flags::none);
        }
    }
}

nlohmann::json ZeroMQServer::processCommand(const nlohmann::json& command, AudioEngine& audioEngine)
{
    nlohmann::json response;

    if (command.contains("action"))
    {
        std::string action = command["action"];
        if (action == "get_devices")
        {
            return audioEngine.getDeviceList();
        }
        else if (action == "set_input" && command.contains("device_name"))
        {
            return audioEngine.setInputDevice(command["device_name"]);
        }
        else if (action == "set_output" && command.contains("device_name"))
        {
            return audioEngine.setOutputDevice(command["device_name"]);
        }
    }

    response["status"] = "error";
    response["message"] = "Unknown or invalid command";
    return response;
}

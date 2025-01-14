#include "ZeroMQServer.h"
#include <iostream>

ZeroMQServer::ZeroMQServer()
    : socket(context, ZMQ_REP)
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
        socket.recv(request, zmq::recv_flags::none);

        try
        {
            std::string msg(static_cast<char*>(request.data()), request.size());
            nlohmann::json command = nlohmann::json::parse(msg);
            nlohmann::json response = processCommand(command);

            zmq::message_t reply(response.dump());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const std::exception& e)
        {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            nlohmann::json errorResponse = {{"status", "error"}, {"message", "Invalid JSON"}};
            zmq::message_t reply(errorResponse.dump());
            socket.send(reply, zmq::send_flags::none);
        }
    }
}

nlohmann::json ZeroMQServer::processCommand(const nlohmann::json& command)
{
    if (command.contains("action") && command["action"] == "get_devices")
    {
        return audioEngine.getDeviceList();
    }
    else if (command.contains("action") && command["action"] == "set_input" && command.contains("device_name"))
    {
        return audioEngine.setInputDevice(command["device_name"].get<std::string>());
    }
    else if (command.contains("action") && command["action"] == "set_output" && command.contains("device_name"))
    {
        return audioEngine.setOutputDevice(command["device_name"].get<std::string>());
    }

    return {{"status", "error"}, {"message", "Unknown or invalid command"}};
}

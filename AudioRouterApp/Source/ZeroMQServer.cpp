#include "ZeroMQServer.h"

ZeroMQServer::ZeroMQServer(AudioEngine& audioEngine)
    : context(1), socket(context, ZMQ_REP), audioEngine(audioEngine)
{
    socket.bind("tcp://*:5555"); // Bind to port 5555
}

ZeroMQServer::~ZeroMQServer() = default;

void ZeroMQServer::listen()
{
    while (true)
    {
        zmq::message_t request;

        try
        {
            auto result = socket.recv(request, zmq::recv_flags::none);
            if (!result)
            {
                std::cerr << "ZeroMQ receive failed: No message received" << std::endl;
                continue;
            }
        }
        catch (const zmq::error_t& e)
        {
            std::cerr << "ZeroMQ receive error: " << e.what() << std::endl;
            continue;
        }

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

#include "ZeroMQServer.h"
#include "AudioEngine.h"

ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine)
{
    try
    {
        socket.bind("tcp://*:5555");
        std::cout << "ZeroMQ server bound to port 5555" << std::endl;
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "Failed to bind ZeroMQ socket: " << e.what() << std::endl;
        throw;
    }
}

nlohmann::json ZeroMQServer::processCommand(const nlohmann::json& command)
{
    if (!command.contains("action") || !command["action"].is_string())
    {
        return {{"status", "error"}, {"message", "Invalid command format"}};
    }

    std::string action = command["action"].get<std::string>();

    if (action == "get_device_list")
    {
        return audioEngine.getDeviceList();
    }
    else if (action == "set_input" && command.contains("device_name") && command["device_name"].is_string())
    {
        std::string deviceName = command["device_name"].get<std::string>();
        return audioEngine.setInputDevice(deviceName);
    }
    else if (action == "set_output" && command.contains("device_name") && command["device_name"].is_string())
    {
        std::string deviceName = command["device_name"].get<std::string>();
        return audioEngine.setOutputDevice(deviceName);
    }
    else
    {
        return {{"status", "error"}, {"message", "Unknown or invalid command"}};
    }
}

void ZeroMQServer::listen()
{
    while (true)
    {
        zmq::message_t request;

        // Receive message
        try
        {
            bool received = socket.recv(request, zmq::recv_flags::none); // Store the return value

            if (!received)
            {
                std::cerr << "ZeroMQ receive failed: No message received" << std::endl;
                continue; // Skip to the next iteration if receiving failed
            }
        }
        catch (const zmq::error_t& e)
        {
            std::cerr << "ZeroMQ receive error: " << e.what() << std::endl;
            continue;
        }

        // Parse incoming message
        try
        {
            std::string msg(static_cast<char*>(request.data()), request.size());
            nlohmann::json command = nlohmann::json::parse(msg);
            nlohmann::json response = processCommand(command);

            // Send response
            zmq::message_t reply(response.dump());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const std::exception& e)
        {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
    }
}

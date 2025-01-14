#include "ZeroMQServer.h"
#include <iostream>
#include <stdexcept>

// Use nlohmann JSON for command parsing and responses
using json = nlohmann::json;

ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine), context(1), socket(context, zmq::socket_type::rep)
{
    try
    {
        // Bind ZeroMQ server to port 5555
        socket.bind("tcp://*:5555");
        std::cout << "ZeroMQ server bound to port 5555" << std::endl;
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "Failed to bind ZeroMQ socket: " << e.what() << std::endl;
        throw; // Rethrow exception to indicate critical failure
    }
}

void ZeroMQServer::listen()
{
    while (true)
    {
        zmq::message_t request;

        try
        {
            // Receive message
            auto bytesReceived = socket.recv(request, zmq::recv_flags::none);
            if (!bytesReceived)
            {
                std::cerr << "ZeroMQServer: Failed to receive message." << std::endl;
                continue;
            }

            // Parse incoming message
            std::string msg(static_cast<char*>(request.data()), request.size());
            json command = json::parse(msg);

            // Prepare response
            json response = processCommand(command);

            // Send response
            zmq::message_t reply(response.dump());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const json::exception& e)
        {
            // Handle JSON parsing errors
            json errorResponse = {
                {"status", "error"},
                {"message", "JSON parsing failed: " + std::string(e.what())}};
            zmq::message_t reply(errorResponse.dump());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const zmq::error_t& e)
        {
            // Handle ZeroMQ errors
            std::cerr << "ZeroMQ error: " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            // Handle other exceptions
            json errorResponse = {
                {"status", "error"},
                {"message", "Exception occurred: " + std::string(e.what())}};
            zmq::message_t reply(errorResponse.dump());
            socket.send(reply, zmq::send_flags::none);
        }
    }
}

json ZeroMQServer::processCommand(const json& command)
{
    if (!command.contains("action") || !command["action"].is_string())
    {
        return {{"status", "error"}, {"message", "Invalid command format"}};
    }

    std::string action = command["action"].get<std::string>();

    if (action == "get_device_list")
    {
        // Return the list of input and output devices
        return audioEngine.getDeviceList();
    }
    else if (action == "set_input")
    {
        if (command.contains("device_name") && command["device_name"].is_string())
        {
            std::string deviceName = command["device_name"].get<std::string>();
            return audioEngine.setInputDevice(deviceName);
        }
        else
        {
            return {{"status", "error"}, {"message", "Missing or invalid 'device_name'"}};
        }
    }
    else if (action == "set_output")
    {
        if (command.contains("device_name") && command["device_name"].is_string())
        {
            std::string deviceName = command["device_name"].get<std::string>();
            return audioEngine.setOutputDevice(deviceName);
        }
        else
        {
            return {{"status", "error"}, {"message", "Missing or invalid 'device_name'"}};
        }
    }
    else
    {
        return {{"status", "error"}, {"message", "Unknown action: " + action}};
    }
}

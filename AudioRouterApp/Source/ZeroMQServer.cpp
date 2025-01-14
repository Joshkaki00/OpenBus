#include "ZeroMQServer.h"
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine), context(1), socket(context, zmq::socket_type::rep)
{
    try
    {
        socket.bind("tcp://*:5555");
        std::cout << "ZeroMQ server bound to port 5555" << std::endl;
    }
    catch (const zmq::error_t& e)
    {
        std::cerr << "Failed to bind ZeroMQ socket: " << e.what() << std::endl;
        throw; // Rethrow the exception to indicate critical failure
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
            auto bytesReceived = socket.recv(request, zmq::recv_flags::none);
            if (!bytesReceived)
            {
                std::cerr << "ZeroMQServer: Failed to receive message." << std::endl;
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
            // Parse incoming message
            std::string msg(static_cast<char*>(request.data()), request.size());
            json command = json::parse(msg);

            json response;

            // Process the command
            if (command.contains("action"))
            {
                std::string action = command["action"].get<std::string>();

                if (action == "get_device_list")
                {
                    response = audioEngine.getDeviceList();
                }
                else if (action == "set_input" && command.contains("device_name"))
                {
                    std::string deviceName = command["device_name"].get<std::string>();
                    response = audioEngine.setInputDevice(deviceName);
                }
                else if (action == "set_output" && command.contains("device_name"))
                {
                    std::string deviceName = command["device_name"].get<std::string>();
                    response = audioEngine.setOutputDevice(deviceName);
                }
                else
                {
                    response = {{"status", "error"}, {"message", "Unknown or invalid command"}};
                }
            }
            else
            {
                response = {{"status", "error"}, {"message", "Invalid command format"}};
            }

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
        catch (const std::exception& e)
        {
            // Handle generic errors
            json errorResponse = {
                {"status", "error"},
                {"message", "Exception occurred: " + std::string(e.what())}};
            zmq::message_t reply(errorResponse.dump());
            socket.send(reply, zmq::send_flags::none);
        }
    }
}

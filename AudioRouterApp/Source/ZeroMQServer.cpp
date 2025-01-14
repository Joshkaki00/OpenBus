#include "ZeroMQServer.h"

ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine), context(1), socket(context, zmq::socket_type::rep)
{
    socket.bind("tcp://*:5555");
}

void ZeroMQServer::listen()
{
    while (true)
    {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);

        std::string msg(static_cast<char*>(request.data()), request.size());
        json command = json::parse(msg);
        json response;

        if (command["action"] == "get_device_list")
        {
            response = audioEngine.getDeviceList();
        }
        else if (command["action"] == "set_input")
        {
            response = audioEngine.setInputDevice(command["device_name"]);
        }
        else if (command["action"] == "set_output")
        {
            response = audioEngine.setOutputDevice(command["device_name"]);
        }
        else
        {
            response = {{"status", "error"}, {"message", "Unknown command"}};
        }

        zmq::message_t reply(response.dump());
        socket.send(reply, zmq::send_flags::none);
    }
}

#include "ZeroMQServer.h"

ZeroMQServer::ZeroMQServer(AudioEngine& engine)
    : audioEngine(engine), context(1), socket(context, zmq::socket_type::rep) {
    socket.bind("tcp://*:5555");
}

void ZeroMQServer::listen() {
    while (true) {
        zmq::message_t request;
        auto recvResult = socket.recv(request, zmq::recv_flags::none); // Capture return value
        if (!recvResult) {
            std::cerr << "Failed to receive message" << std::endl;
            continue; // Skip to the next iteration if the receive fails
        }

        std::string msg(static_cast<char*>(request.data()), request.size());
        json command = json::parse(msg);
        json response;

        if (command["action"] == "get_device_list") {
            response = audioEngine.getDeviceList();
        } else if (command["action"] == "set_input") {
            response = audioEngine.setInputDevice(command["device_name"]);
        } else if (command["action"] == "set_output") {
            response = audioEngine.setOutputDevice(command["device_name"]);
        } else {
            response = {{"status", "error"}, {"message", "Unknown command"}};
        }

        zmq::message_t reply(response.dump());
        auto sendResult = socket.send(reply, zmq::send_flags::none); // Capture return value
        if (!sendResult) {
            std::cerr << "Failed to send response" << std::endl;
        }
    }
}

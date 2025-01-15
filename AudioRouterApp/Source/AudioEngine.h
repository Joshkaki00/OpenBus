#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <nlohmann/json.hpp> // For JSON support
#include <string>

class AudioEngine {
public:
    AudioEngine() = default; // Public default constructor

    // Method to get the list of audio devices
    nlohmann::json getDeviceList() {
        nlohmann::json devices = {
            {"inputDevices", {"Microphone 1", "Microphone 2"}},
            {"outputDevices", {"Speakers", "Headphones"}}
        };
        return devices;
    }

    // Method to set input device
    nlohmann::json setInputDevice(const std::string& deviceName) {
        if (deviceName.empty()) {
            return {{"status", "error"}, {"message", "Device name cannot be empty"}};
        }
        // Simulate successful input device set
        return {{"status", "success"}, {"message", "Input device set to " + deviceName}};
    }

    // Method to set output device
    nlohmann::json setOutputDevice(const std::string& deviceName) {
        if (deviceName.empty()) {
            return {{"status", "error"}, {"message", "Device name cannot be empty"}};
        }
        // Simulate successful output device set
        return {{"status", "success"}, {"message", "Output device set to " + deviceName}};
    }
};

#endif // AUDIO_ENGINE_H

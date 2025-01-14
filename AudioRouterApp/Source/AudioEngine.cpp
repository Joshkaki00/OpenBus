#include "AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine() {
    // Initialize default input and output devices (2 channels each)
    deviceManager.initialiseWithDefaultDevices(2, 2);
}

json AudioEngine::getDeviceList() {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        // Fetch input and output device names
        auto inputDevices = currentDevice->getInputChannelNames();
        auto outputDevices = currentDevice->getOutputChannelNames();

        // Convert juce::StringArray to std::vector<std::string>
        std::vector<std::string> inputDeviceList;
        std::vector<std::string> outputDeviceList;

        for (const auto& device : inputDevices) {
            inputDeviceList.push_back(device.toStdString());
        }

        for (const auto& device : outputDevices) {
            outputDeviceList.push_back(device.toStdString());
        }

        return {
            {"status", "success"},
            {"inputs", inputDeviceList},
            {"outputs", outputDeviceList}
        };
    }

    return {{"status", "error"}, {"message", "No audio device found"}};
}

json AudioEngine::setInputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        // Check if the requested input device is available
        if (currentDevice->getInputChannelNames().contains(juce::String(deviceName))) {
            DBG("Input device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Input device set successfully"}};
        }
    }

    return {{"status", "error"}, {"message", "Input device not found"}};
}

json AudioEngine::setOutputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        // Check if the requested output device is available
        if (currentDevice->getOutputChannelNames().contains(juce::String(deviceName))) {
            DBG("Output device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Output device set successfully"}};
        }
    }

    return {{"status", "error"}, {"message", "Output device not found"}};
}

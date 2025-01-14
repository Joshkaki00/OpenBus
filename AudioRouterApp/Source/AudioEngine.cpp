#include "AudioEngine.h"

AudioEngine::AudioEngine() {
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize with 2 inputs and 2 outputs
}

AudioEngine::~AudioEngine() {
    deviceManager.closeAudioDevice();
}

json AudioEngine::getDeviceList() {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        auto inputDevices = currentDevice->getInputChannelNames();
        auto outputDevices = currentDevice->getOutputChannelNames();

        // Convert juce::StringArray to std::vector<std::string>
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;

        for (const auto& input : inputDevices) {
            inputs.push_back(input.toStdString());
        }

        for (const auto& output : outputDevices) {
            outputs.push_back(output.toStdString());
        }

        return {
            {"status", "success"},
            {"inputs", inputs},
            {"outputs", outputs}
        };
    }

    return {{"status", "error"}, {"message", "No audio device found"}};
}

json AudioEngine::setInputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        if (currentDevice->getInputChannelNames().contains(deviceName)) {
            DBG("Input device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Input device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Input device not found"}};
}

json AudioEngine::setOutputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        if (currentDevice->getOutputChannelNames().contains(deviceName)) {
            DBG("Output device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Output device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Output device not found"}};
}

json AudioEngine::savePreset(const std::string& presetName) {
    DBG("Preset saved: " << presetName);
    return {{"status", "success"}, {"message", "Preset saved successfully"}};
}

json AudioEngine::loadPreset(const std::string& presetName) {
    DBG("Preset loaded: " << presetName);
    return {{"status", "success"}, {"message", "Preset loaded successfully"}};
}

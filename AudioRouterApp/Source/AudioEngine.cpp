#include "AudioEngine.h"
#include <fstream> // For file operations

AudioEngine::AudioEngine() {
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize with default input/output
}

json AudioEngine::getDeviceList() {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        auto inputDevices = currentDevice->getInputChannelNames();
        auto outputDevices = currentDevice->getOutputChannelNames();

        // Convert JUCE StringArray to std::vector<std::string>
        std::vector<std::string> inputs(inputDevices.begin(), inputDevices.end());
        std::vector<std::string> outputs(outputDevices.begin(), outputDevices.end());

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
        auto inputDevices = currentDevice->getInputChannelNames();
        if (inputDevices.contains(deviceName)) {
            DBG("Input device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Input device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Input device not found"}};
}

json AudioEngine::setOutputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        auto outputDevices = currentDevice->getOutputChannelNames();
        if (outputDevices.contains(deviceName)) {
            DBG("Output device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Output device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Output device not found"}};
}

json AudioEngine::savePreset(const std::string& presetName) {
    try {
        json presetData;

        if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
            presetData["inputChannels"] = currentDevice->getInputChannelNames().toStdString();
            presetData["outputChannels"] = currentDevice->getOutputChannelNames().toStdString();
        }

        std::ofstream file(presetName + ".json");
        if (!file.is_open()) {
            return {{"status", "error"}, {"message", "Failed to open preset file for writing"}};
        }
        file << presetData.dump(4); // Save JSON with indentation
        file.close();

        return {{"status", "success"}, {"message", "Preset saved successfully"}};
    } catch (const std::exception& e) {
        return {{"status", "error"}, {"message", e.what()}};
    }
}

json AudioEngine::loadPreset(const std::string& presetName) {
    try {
        std::ifstream file(presetName + ".json");
        if (!file.is_open()) {
            return {{"status", "error"}, {"message", "Failed to open preset file for reading"}};
        }

        json presetData;
        file >> presetData;
        file.close();

        if (presetData.contains("inputChannels") && presetData.contains("outputChannels")) {
            DBG("Preset loaded: Inputs - " << presetData["inputChannels"]);
            DBG("Preset loaded: Outputs - " << presetData["outputChannels"]);
            return {{"status", "success"}, {"message", "Preset loaded successfully"}};
        } else {
            return {{"status", "error"}, {"message", "Invalid preset format"}};
        }
    } catch (const std::exception& e) {
        return {{"status", "error"}, {"message", e.what()}};
    }
}

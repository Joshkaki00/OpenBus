#include "AudioEngine.h"
#include <nlohmann/json.hpp>
#include <juce_audio_devices/juce_audio_devices.h>

using json = nlohmann::json;

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    // Initialize any necessary components
}

bool AudioEngine::loadPlugin(const juce::File& file)
{
    // Add logic to load the plugin
    return file.existsAsFile(); // Placeholder: Replace with actual plugin loading logic
}

bool AudioEngine::savePreset(const juce::File& file)
{
    // Add logic to save the preset
    return file.create(); // Placeholder: Replace with actual preset saving logic
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    // Add logic to load the preset
    return file.existsAsFile(); // Placeholder: Replace with actual preset loading logic
}

nlohmann::json AudioEngine::getDeviceList() {
    nlohmann::json response;

    // Get the list of available device types
    juce::OwnedArray<juce::AudioIODeviceType> types;
    deviceManager.createAudioDeviceTypes(types);  // Ensure deviceManager is not const

    // Iterate through device types
    for (auto* type : types) {
        type->scanForDevices();  // Refresh device list
        auto inputDevices = type->getDeviceNames(true);   // Input devices
        auto outputDevices = type->getDeviceNames(false); // Output devices

        // Convert JUCE StringArray to std::vector<std::string>
        std::vector<std::string> inputs;
        for (const auto& input : inputDevices) {
            inputs.push_back(input.toStdString());
        }

        std::vector<std::string> outputs;
        for (const auto& output : outputDevices) {
            outputs.push_back(output.toStdString());
        }

        // Assign the vectors to JSON
        response["inputs"] = inputs;
        response["outputs"] = outputs;
    }

    return response;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName) {
    nlohmann::json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.inputDeviceName = deviceName;

    auto error = deviceManager.setAudioDeviceSetup(setup, true);
    if (error.isEmpty()) { // No error indicates success
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    } else {
        response["status"] = "error";
        response["message"] = "Failed to set input device: " + error.toStdString();
    }

    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName) {
    nlohmann::json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = deviceName;

    auto error = deviceManager.setAudioDeviceSetup(setup, true);
    if (error.isEmpty()) { // No error indicates success
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    } else {
        response["status"] = "error";
        response["message"] = "Failed to set output device: " + error.toStdString();
    }

    return response;
}

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize with 2 inputs and 2 outputs
}

AudioEngine::~AudioEngine() = default;

bool AudioEngine::loadPlugin(const juce::File& file)
{
    // TODO: Implement plugin loading logic
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    // TODO: Implement preset saving logic
    return true;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    // TODO: Implement preset loading logic
    return true;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;
    nlohmann::json inputs, outputs;

    // Get available device types from the device manager
    auto deviceTypes = deviceManager.getAvailableDeviceTypes();
    for (const auto& type : *deviceTypes)
    {
        type->scanForDevices(); // Scan for available devices
        auto inputNames = type->getDeviceNames(true);  // Get input devices
        auto outputNames = type->getDeviceNames(false); // Get output devices

        // Populate input and output JSON arrays
        for (const auto& input : inputNames)
            inputs.push_back(input.toStdString());

        for (const auto& output : outputNames)
            outputs.push_back(output.toStdString());
    }

    devices["inputs"] = inputs;
    devices["outputs"] = outputs;

    return devices;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup); // Get current audio device setup

    setup.inputDeviceName = deviceName; // Set the input device name
    auto result = deviceManager.setAudioDeviceSetup(setup, true); // Apply the setup

    if (result.failed())
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString(); // Return error message
    }
    else
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully"; // Success message
    }

    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup); // Get current audio device setup

    setup.outputDeviceName = deviceName; // Set the output device name
    auto result = deviceManager.setAudioDeviceSetup(setup, true); // Apply the setup

    if (result.failed())
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString(); // Return error message
    }
    else
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully"; // Success message
    }

    return response;
}

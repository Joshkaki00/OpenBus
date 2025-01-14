#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    // Initialize the audio device manager with default devices (2 inputs, 2 outputs)
    deviceManager.initialiseWithDefaultDevices(2, 2);
}

AudioEngine::~AudioEngine() = default;

bool AudioEngine::loadPlugin(const juce::File& file)
{
    // Placeholder for plugin loading logic
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    // Placeholder for saving preset logic
    return true;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    // Placeholder for loading preset logic
    return true;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;
    nlohmann::json inputs, outputs;

    // Get available device types
    auto& deviceTypes = deviceManager.getAvailableDeviceTypes();
    for (auto* type : deviceTypes)
    {
        type->scanForDevices();

        // Retrieve input and output device names
        auto inputNames = type->getDeviceNames(true);
        auto outputNames = type->getDeviceNames(false);

        for (const auto& input : inputNames)
            inputs.push_back(input.toStdString());

        for (const auto& output : outputNames)
            outputs.push_back(output.toStdString());
    }

    // Populate JSON with input and output device names
    devices["inputs"] = inputs;
    devices["outputs"] = outputs;

    return devices;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    nlohmann::json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    // Set the input device name
    setup.inputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.wasOk())
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString();
    }

    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    nlohmann::json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    // Set the output device name
    setup.outputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.wasOk())
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString();
    }

    return response;
}

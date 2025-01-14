#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialise(2, 2, nullptr, true); // Initialize device manager with 2 inputs and 2 outputs
}

AudioEngine::~AudioEngine()
{
    // Destructor to clean up resources if needed
}

bool AudioEngine::loadPlugin(const juce::File& file)
{
    // Add plugin loading implementation here
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    // Add preset saving logic here
    return true;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    // Add preset loading logic here
    return true;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;

    auto types = deviceManager.getAvailableDeviceTypes();
    for (auto* type : types)
    {
        nlohmann::json inputs;
        nlohmann::json outputs;

        // Get input devices
        for (auto& input : type->getDeviceNames())
        {
            inputs.push_back(input.toStdString());
        }

        // Get output devices
        for (auto& output : type->getDeviceNames())
        {
            outputs.push_back(output.toStdString());
        }

        devices["inputs"] = inputs;
        devices["outputs"] = outputs;
    }

    return devices;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    auto result = deviceManager.setAudioDeviceSetup(deviceManager.getAudioDeviceSetup(), true);

    nlohmann::json response;
    if (result.failed())
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString();
    }
    else
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    auto result = deviceManager.setAudioDeviceSetup(deviceManager.getAudioDeviceSetup(), true);

    nlohmann::json response;
    if (result.failed())
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString();
    }
    else
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    return response;
}

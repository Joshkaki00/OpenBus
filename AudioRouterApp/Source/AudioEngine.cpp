#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialiseWithDefaultDevices(2, 2); // Example: 2 input, 2 output
    deviceTypes = deviceManager.createAudioDeviceTypes(); // Populate device types
}

AudioEngine::~AudioEngine() = default;

bool AudioEngine::loadPlugin(const juce::File& file)
{
    // Implement plugin loading logic here
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    // Implement preset saving logic here
    return true;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    // Implement preset loading logic here
    return true;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;
    nlohmann::json inputs, outputs;

    for (auto& type : deviceTypes)
    {
        type->scanForDevices();
        auto inputNames = type->getDeviceNames(true);
        auto outputNames = type->getDeviceNames(false);

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

    auto result = deviceManager.setAudioDeviceSetup(deviceManager.getAudioDeviceSetup(), true);
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
    nlohmann::json response;

    auto result = deviceManager.setAudioDeviceSetup(deviceManager.getAudioDeviceSetup(), true);
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

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialise(2, 2, nullptr, true);
}

AudioEngine::~AudioEngine() = default;

nlohmann::json AudioEngine::getDeviceList() const
{
    nlohmann::json response;
    auto setup = deviceManager.getAudioDeviceSetup();
    auto deviceType = deviceManager.getCurrentDeviceTypeObject();

    if (deviceType != nullptr)
    {
        auto deviceNames = deviceType->getDeviceNames();
        response["status"] = "success";
        for (auto& name : deviceNames)
        {
            response["devices"].push_back(name.toStdString());
        }
    }
    else
    {
        response["status"] = "error";
        response["message"] = "No devices found";
    }

    return response;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.inputDeviceName = deviceName;

    bool success = deviceManager.setAudioDeviceSetup(setup, true); // This returns a bool
    if (success)
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set input device"; // Generic error message
    }

    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = deviceName;

    bool success = deviceManager.setAudioDeviceSetup(setup, true); // This returns a bool
    if (success)
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set output device"; // Generic error message
    }

    return response;
}

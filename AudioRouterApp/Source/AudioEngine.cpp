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

    // Attempt to set the input device and verify success through a follow-up query
    deviceManager.setAudioDeviceSetup(setup, true);

    // Re-fetch the setup to confirm the change
    juce::AudioDeviceManager::AudioDeviceSetup updatedSetup;
    deviceManager.getAudioDeviceSetup(updatedSetup);

    if (updatedSetup.inputDeviceName == deviceName)
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set input device";
    }

    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = deviceName;

    // Attempt to set the output device and verify success through a follow-up query
    deviceManager.setAudioDeviceSetup(setup, true);

    // Re-fetch the setup to confirm the change
    juce::AudioDeviceManager::AudioDeviceSetup updatedSetup;
    deviceManager.getAudioDeviceSetup(updatedSetup);

    if (updatedSetup.outputDeviceName == deviceName)
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set output device";
    }

    return response;
}

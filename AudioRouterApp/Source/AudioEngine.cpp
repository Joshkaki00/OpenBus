#include "AudioEngine.h"
#include <nlohmann/json.hpp>
#include <juce_audio_devices/juce_audio_devices.h>

using json = nlohmann::json;

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

json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.inputDeviceName = juce::String(deviceName); // Convert std::string to juce::String

    // Attempt to set the input device
    deviceManager.setAudioDeviceSetup(setup, true);

    // Re-fetch the setup to confirm the change
    juce::AudioDeviceManager::AudioDeviceSetup updatedSetup;
    deviceManager.getAudioDeviceSetup(updatedSetup);

    // Verify if the input device was correctly set
    if (updatedSetup.inputDeviceName == juce::String(deviceName))
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

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = juce::String(deviceName); // Convert std::string to juce::String

    // Attempt to set the output device
    deviceManager.setAudioDeviceSetup(setup, true);

    // Re-fetch the setup to confirm the change
    juce::AudioDeviceManager::AudioDeviceSetup updatedSetup;
    deviceManager.getAudioDeviceSetup(updatedSetup);

    // Verify if the output device was correctly set
    if (updatedSetup.outputDeviceName == juce::String(deviceName))
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

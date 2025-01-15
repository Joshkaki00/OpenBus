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
    auto types = juce::AudioIODeviceType::createAudioIODeviceTypeInstanceList();
    for (auto* type : types)
    {
        type->scanForDevices();
        auto inputDevices = type->getDeviceNames(true);
        auto outputDevices = type->getDeviceNames(false);

        response["inputs"] = inputDevices;
        response["outputs"] = outputDevices;
    }
    return response;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;
    if (deviceManager.setAudioDeviceSetup(setup, true))
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
    if (deviceManager.setAudioDeviceSetup(setup, true))
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

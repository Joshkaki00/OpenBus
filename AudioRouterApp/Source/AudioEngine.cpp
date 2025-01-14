#include "AudioEngine.h"

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;
    juce::AudioDeviceManager deviceManager;

    // Retrieve available device types
    auto deviceTypes = deviceManager.getAvailableDeviceTypes();

    nlohmann::json inputs, outputs;

    for (const auto* type : *deviceTypes)
    {
        type->scanForDevices();

        auto inputNames = type->getDeviceNames(true);  // True for input devices
        auto outputNames = type->getDeviceNames(false); // False for output devices

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
    juce::AudioDeviceManager deviceManager;

    deviceManager.getAudioDeviceSetup(setup);
    setup.inputDeviceName = deviceName;

    bool setupSuccess = deviceManager.setAudioDeviceSetup(setup, true); // Configure input device
    if (setupSuccess)
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
    juce::AudioDeviceManager deviceManager;

    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = deviceName;

    bool setupSuccess = deviceManager.setAudioDeviceSetup(setup, true); // Configure output device
    if (setupSuccess)
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

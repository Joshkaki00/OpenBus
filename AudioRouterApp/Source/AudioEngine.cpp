#include "AudioEngine.h"

// Method to fetch available audio devices
json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    // Retrieve available device types
    auto deviceTypes = deviceManager.getAvailableDeviceTypes();

    for (const auto& type : deviceTypes)
    {
        type->scanForDevices();

        // Get input and output device names
        auto inputNames = type->getDeviceNames(true);  // true for input devices
        auto outputNames = type->getDeviceNames(false); // false for output devices

        for (const auto& input : inputNames)
            inputs.push_back(input.toStdString());

        for (const auto& output : outputNames)
            outputs.push_back(output.toStdString());
    }

    devices["inputs"] = inputs;
    devices["outputs"] = outputs;
    return devices;
}

// Method to set input device
json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;

    // Set input device and check success
    bool setupSuccess = deviceManager.setAudioDeviceSetup(setup, true);
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

// Method to set output device
json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.outputDeviceName = deviceName;

    // Set output device and check success
    bool setupSuccess = deviceManager.setAudioDeviceSetup(setup, true);
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

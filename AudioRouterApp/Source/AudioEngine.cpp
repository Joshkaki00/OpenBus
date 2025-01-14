#include "AudioEngine.h"

json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    // Retrieve available device types
    auto& deviceTypes = deviceManager.getAvailableDeviceTypes(); // Ensure non-const compatibility

    for (auto* type : deviceTypes) // Access each AudioIODeviceType
    {
        type->scanForDevices(); // Scan for devices

        auto inputNames = type->getDeviceNames(true);  // True for input devices
        auto outputNames = type->getDeviceNames(false); // False for output devices

        for (const auto& input : inputNames)
            inputs.push_back(input.toStdString()); // Convert to std::string

        for (const auto& output : outputNames)
            outputs.push_back(output.toStdString()); // Convert to std::string
    }

    devices["inputs"] = inputs;
    devices["outputs"] = outputs;
    return devices;
}

json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;

    // Set up the device and check success
    bool setupSuccess = deviceManager.setAudioDeviceSetup(setup, true); // Configure input device
    if (setupSuccess)
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set input device"; // Provide generic error message
    }

    return response;
}

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.outputDeviceName = deviceName;

    // Set up the device and check success
    bool setupSuccess = deviceManager.setAudioDeviceSetup(setup, true); // Configure output device
    if (setupSuccess)
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = "Failed to set output device"; // Provide generic error message
    }

    return response;
}

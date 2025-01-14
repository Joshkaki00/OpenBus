#include "AudioEngine.h"

json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    // Retrieve available device types
    auto& deviceTypes = deviceManager.getAvailableDeviceTypes(); // Remove const

    for (auto* type : deviceTypes) // Access each AudioIODeviceType
    {
        type->scanForDevices(); // No const issues

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

json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;

    auto result = deviceManager.setAudioDeviceSetup(setup, true); // Configure input device
    if (result.wasOk()) // Check for success
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString(); // Error details
    }

    return response;
}

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.outputDeviceName = deviceName;

    auto result = deviceManager.setAudioDeviceSetup(setup, true); // Configure output device
    if (result.wasOk()) // Check for success
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString(); // Error details
    }

    return response;
}

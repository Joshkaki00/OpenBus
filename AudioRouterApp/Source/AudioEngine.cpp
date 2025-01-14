#include "AudioEngine.h"

json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    auto* deviceTypes = deviceManager.getAvailableDeviceTypes();

    for (const auto* type : *deviceTypes) // Ensure type is valid and non-const
    {
        type->scanForDevices(); // Adjust if const qualifier is required

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

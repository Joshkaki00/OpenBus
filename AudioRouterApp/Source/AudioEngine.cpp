#include "AudioEngine.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    // Use a reference to avoid copying the OwnedArray
    auto& deviceTypes = deviceManager.getAvailableDeviceTypes();

    for (int i = 0; i < deviceTypes.size(); ++i)
    {
        auto* type = deviceTypes[i];
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

json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.wasOk()) // Check for success
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString();
    }

    return response;
}

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.outputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.wasOk()) // Check for success
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString();
    }

    return response;
}

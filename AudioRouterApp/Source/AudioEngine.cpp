#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize with 2 input and 2 output channels
}

AudioEngine::~AudioEngine() = default;

bool AudioEngine::loadPlugin(const juce::File& file)
{
    // Plugin loading logic
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    // Preset saving logic
    return true;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    // Preset loading logic
    return true;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;
    nlohmann::json inputs, outputs;

    auto deviceTypes = deviceManager.getAvailableDeviceTypes();
    for (const auto& type : *deviceTypes)
    {
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

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    nlohmann::json response;
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

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    nlohmann::json response;
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

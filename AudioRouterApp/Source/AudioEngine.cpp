#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialise(2, 2, nullptr, true);
}

AudioEngine::~AudioEngine() {}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json devices;
    nlohmann::json inputs;
    nlohmann::json outputs;

    auto deviceTypes = deviceManager.getAvailableDeviceTypes();
    for (auto* type : *deviceTypes)
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

    if (result)
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

    if (result)
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

nlohmann::json AudioEngine::loadPlugin(const juce::File& file)
{
    nlohmann::json response;
    if (!file.existsAsFile())
    {
        response["status"] = "error";
        response["message"] = "Plugin file does not exist.";
        return response;
    }

    // Plugin loading logic here
    response["status"] = "success";
    response["message"] = "Plugin loaded successfully.";
    return response;
}

nlohmann::json AudioEngine::savePreset(const juce::File& file)
{
    nlohmann::json response;
    if (file.getFullPathName().isEmpty())
    {
        response["status"] = "error";
        response["message"] = "Invalid preset file.";
        return response;
    }

    // Preset saving logic here
    response["status"] = "success";
    response["message"] = "Preset saved successfully.";
    return response;
}

nlohmann::json AudioEngine::loadPreset(const juce::File& file)
{
    nlohmann::json response;
    if (!file.existsAsFile())
    {
        response["status"] = "error";
        response["message"] = "Preset file does not exist.";
        return response;
    }

    // Preset loading logic here
    response["status"] = "success";
    response["message"] = "Preset loaded successfully.";
    return response;
}

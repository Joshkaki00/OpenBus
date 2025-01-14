#include "AudioEngine.h"
#include <fstream>

AudioEngine::AudioEngine()
{
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize input/output devices
}

AudioEngine::~AudioEngine() {}

void AudioEngine::savePreset(const std::string& presetName)
{
    nlohmann::json preset;

    // Save device settings
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        preset["inputDevice"] = currentDevice->getInputChannelNames().joinIntoString(", ").toStdString();
        preset["outputDevice"] = currentDevice->getOutputChannelNames().joinIntoString(", ").toStdString();
    }

    // Save preset to file
    std::ofstream file(presetName + ".json");
    if (file.is_open())
    {
        file << preset.dump(4);
        file.close();
    }
}

void AudioEngine::loadPreset(const std::string& presetName)
{
    std::ifstream file(presetName + ".json");
    if (!file.is_open())
        return;

    nlohmann::json preset;
    file >> preset;
    file.close();

    // Load device settings
    auto inputDevice = preset.value("inputDevice", "");
    auto outputDevice = preset.value("outputDevice", "");

    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        if (currentDevice->getInputChannelNames().contains(inputDevice))
        {
            setInputDevice(inputDevice);
        }

        if (currentDevice->getOutputChannelNames().contains(outputDevice))
        {
            setOutputDevice(outputDevice);
        }
    }
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json deviceList;

    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        auto inputDevices = currentDevice->getInputChannelNames();
        auto outputDevices = currentDevice->getOutputChannelNames();

        for (auto& input : inputDevices)
        {
            deviceList["inputs"].push_back(input.toStdString());
        }

        for (auto& output : outputDevices)
        {
            deviceList["outputs"].push_back(output.toStdString());
        }
    }

    return deviceList;
}

void AudioEngine::setInputDevice(const std::string& deviceName)
{
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        if (currentDevice->getInputChannelNames().contains(deviceName))
        {
            DBG("Input device set to: " << deviceName);
        }
    }
}

void AudioEngine::setOutputDevice(const std::string& deviceName)
{
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        if (currentDevice->getOutputChannelNames().contains(deviceName))
        {
            DBG("Output device set to: " << deviceName);
        }
    }
}

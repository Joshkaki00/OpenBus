#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize default input/output devices
}

json AudioEngine::getDeviceList()
{
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        auto inputDevices = currentDevice->getInputChannelNames();
        auto outputDevices = currentDevice->getOutputChannelNames();

        std::vector<std::string> inputs;
        std::vector<std::string> outputs;

        for (const auto& input : inputDevices)
            inputs.push_back(input.toStdString());
        for (const auto& output : outputDevices)
            outputs.push_back(output.toStdString());

        return {
            {"status", "success"},
            {"inputs", inputs},
            {"outputs", outputs}
        };
    }
    return {{"status", "error"}, {"message", "No audio device found"}};
}

json AudioEngine::setInputDevice(const std::string& deviceName)
{
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        auto inputDevices = currentDevice->getInputChannelNames();
        if (inputDevices.contains(juce::String(deviceName)))
        {
            DBG("Input device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Input device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Input device not found"}};
}

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
    {
        auto outputDevices = currentDevice->getOutputChannelNames();
        if (outputDevices.contains(juce::String(deviceName)))
        {
            DBG("Output device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Output device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Output device not found"}};
}

json AudioEngine::savePreset(const std::string& presetName)
{
    try
    {
        json presetData;

        if (auto* currentDevice = deviceManager.getCurrentAudioDevice())
        {
            std::vector<std::string> inputChannels;
            std::vector<std::string> outputChannels;

            // Convert JUCE::StringArray to std::vector<std::string>
            for (const auto& input : currentDevice->getInputChannelNames())
                inputChannels.push_back(input.toStdString());

            for (const auto& output : currentDevice->getOutputChannelNames())
                outputChannels.push_back(output.toStdString());

            presetData["inputChannels"] = inputChannels;
            presetData["outputChannels"] = outputChannels;
        }

        std::ofstream file(presetName + ".json");
        if (!file.is_open())
            return {{"status", "error"}, {"message", "Failed to open preset file for writing"}};

        file << presetData.dump(4); // Save JSON with indentation
        file.close();

        return {{"status", "success"}, {"message", "Preset saved successfully"}};
    }
    catch (const std::exception& e)
    {
        return {{"status", "error"}, {"message", e.what()}};
    }
}

json AudioEngine::loadPreset(const std::string& presetName)
{
    try
    {
        std::ifstream file(presetName + ".json");
        if (!file.is_open())
            return {{"status", "error"}, {"message", "Failed to open preset file for reading"}};

        json presetData;
        file >> presetData;
        file.close();

        if (presetData.contains("inputChannels") && presetData.contains("outputChannels"))
        {
            DBG("Preset loaded: Inputs - " << presetData["inputChannels"].dump());
            DBG("Preset loaded: Outputs - " << presetData["outputChannels"].dump());
            return {{"status", "success"}, {"message", "Preset loaded successfully"}};
        }

        return {{"status", "error"}, {"message", "Invalid preset format"}};
    }
    catch (const std::exception& e)
    {
        return {{"status", "error"}, {"message", e.what()}};
    }
}

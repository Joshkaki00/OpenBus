#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    nlohmann::json getDeviceList();
    nlohmann::json setInputDevice(const std::string& deviceName);
    nlohmann::json setOutputDevice(const std::string& deviceName);

    nlohmann::json loadPlugin(const juce::File& file);
    nlohmann::json savePreset(const juce::File& file);
    nlohmann::json loadPreset(const juce::File& file);

private:
    juce::AudioDeviceManager deviceManager;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

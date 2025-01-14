#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    AudioEngine() = default;
    ~AudioEngine() = default;

    nlohmann::json getDeviceList();
    nlohmann::json setInputDevice(const std::string& deviceName);
    nlohmann::json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

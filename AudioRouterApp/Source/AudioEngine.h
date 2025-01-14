#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    json getDeviceList();
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

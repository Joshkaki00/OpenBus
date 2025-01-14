#pragma once

#include <JuceHeader.h>
#include <string>
#include <nlohmann/json.hpp> // Assuming you're using JSON for saving presets

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    void savePreset(const std::string& presetName);
    void loadPreset(const std::string& presetName);

    nlohmann::json getDeviceList();
    void setInputDevice(const std::string& deviceName);
    void setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
};

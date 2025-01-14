#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>

using json = nlohmann::json;

class AudioEngine
{
public:
    AudioEngine();

    json getDeviceList();
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);
    json savePreset(const std::string& presetName);
    json loadPreset(const std::string& presetName);

private:
    juce::AudioDeviceManager deviceManager;
};

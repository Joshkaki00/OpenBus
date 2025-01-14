#pragma once

#include <juce_audio_devices/juce_audio_devices.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioEngine {
public:
    AudioEngine();

    // Core functionalities
    json getDeviceList();
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);

    // Preset management
    json savePreset(const std::string& presetName);
    json loadPreset(const std::string& presetName);

private:
    juce::AudioDeviceManager deviceManager;
};

#pragma once
#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <juce_core/juce_core.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <nlohmann/json.hpp>

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine() = default;
    
    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

    nlohmann::json getDeviceList();
    nlohmann::json setInputDevice(const std::string& deviceName);
    nlohmann::json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
};


#endif // AUDIOENGINE_H

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <juce_audio_devices/juce_audio_devices.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class AudioEngine
{
public:
    AudioEngine() = default;
    ~AudioEngine() = default;

    // Method to fetch available audio devices
    json getDeviceList();

    // Methods to set input and output devices
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
};

#endif // AUDIO_ENGINE_H

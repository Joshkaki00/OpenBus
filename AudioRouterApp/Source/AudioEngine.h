#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <juce_core/juce_core.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    // Constructor and Destructor
    AudioEngine();
    ~AudioEngine();

    // Device methods
    nlohmann::json getDeviceList() const;
    nlohmann::json setInputDevice(const std::string& deviceName);
    nlohmann::json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
};

#endif // AUDIOENGINE_H

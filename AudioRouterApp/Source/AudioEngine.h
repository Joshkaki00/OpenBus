#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <nlohmann/json.hpp>
#include <juce_audio_devices/juce_audio_devices.h>

using json = nlohmann::json;

class AudioEngine {
public:
    AudioEngine();
    json getDeviceList();
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
};

#endif // AUDIOENGINE_H

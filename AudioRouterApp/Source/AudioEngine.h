#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <JuceHeader.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

class AudioEngine {
public:
    AudioEngine();
    ~AudioEngine() = default;

    // Fetch available input and output devices
    json getDeviceList();

    // Set input and output devices by name
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager; // JUCE's audio device manager
};

#endif // AUDIOENGINE_H

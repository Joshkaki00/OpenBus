#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <juce_core/juce_core.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    static AudioEngine& getInstance()
    {
        static AudioEngine instance; // Guaranteed to be destroyed and instantiated on first use
        return instance;
    }

    // Prevent copying and assignment
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    // Add your existing methods like setInputDevice, setOutputDevice, loadPlugin, savePreset, etc.
    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    AudioEngine() = default; // Private constructor
};

#endif // AUDIOENGINE_H

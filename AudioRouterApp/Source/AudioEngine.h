#pragma once
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
        static AudioEngine instance; // Thread-safe in C++11 and later
        return instance;
    }

    // Methods for plugin and preset handling
    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    AudioEngine(); // Private constructor
    ~AudioEngine() = default;

    // Delete copy constructor and assignment operator to prevent copies
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;

    juce::AudioDeviceManager deviceManager; // Example member variable
};

#endif // AUDIOENGINE_H

#pragma once

#include <JuceHeader.h>

class AudioEngine
{
public:
    static AudioEngine& getInstance()
    {
        static AudioEngine instance;
        return instance;
    }

    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    AudioEngine() = default;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

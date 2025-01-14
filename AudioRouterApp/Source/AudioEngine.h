#pragma once

#include <JuceHeader.h>

class AudioEngine
{
public:
    AudioEngine() = default;
    ~AudioEngine() = default;

    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

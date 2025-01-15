#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

class AudioEngine
{
public:
    // Constructor and Destructor
    AudioEngine() = default;
    ~AudioEngine() = default;

    // Plugin and Preset Methods
    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    // Add any private members or methods here if needed
};

#endif // AUDIOENGINE_H

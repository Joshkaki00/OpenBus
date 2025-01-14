#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

/**
 * AudioEngine handles audio-related functionalities, including loading plugins
 * and saving/loading presets.
 */
class AudioEngine
{
public:
    AudioEngine() = default;

    /** Loads a plugin from the specified file.
        @param file The file to load the plugin from.
        @return True if the plugin was loaded successfully, false otherwise.
    */
    bool loadPlugin(const juce::File& file);

    /** Saves a preset to the specified file.
        @param file The file to save the preset to.
        @return True if the preset was saved successfully, false otherwise.
    */
    bool savePreset(const juce::File& file);

    /** Loads a preset from the specified file.
        @param file The file to load the preset from.
        @return True if the preset was loaded successfully, false otherwise.
    */
    bool loadPreset(const juce::File& file);
};

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    static AudioEngine& getInstance()
    {
        static AudioEngine instance;
        return instance;
    }

    AudioEngine();
    ~AudioEngine();

    bool loadPlugin(const juce::File& file);              // Load a plugin from a file
    void clearPlugins();                                  // Unload all plugins
    juce::AudioProcessorEditor* createEditorForPlugin(int index); // Create an editor for a loaded plugin
    nlohmann::json getDeviceList();                      // Get audio input/output devices
    nlohmann::json setInputDevice(const std::string& deviceName);
    nlohmann::json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioPluginFormatManager pluginFormatManager;
    juce::OwnedArray<juce::AudioPluginInstance> loadedPlugins;
    juce::AudioDeviceManager deviceManager;
};

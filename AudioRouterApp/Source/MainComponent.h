#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component
{
public:
    explicit MainComponent(AudioEngine& audioEngine);
    ~MainComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Reference to the audio engine
    AudioEngine& audioEngine;

    // GUI elements
    juce::TextButton loadPluginButton { "Load Plugin" };
    juce::TextButton savePresetButton { "Save Preset" };
    juce::TextButton loadPresetButton { "Load Preset" };

    // Button click handlers
    void loadPlugin();
    void savePreset();
    void loadPreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

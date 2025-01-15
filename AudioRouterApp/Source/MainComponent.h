#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component
{
public:
    // Constructor and Destructor
    MainComponent();
    ~MainComponent() override = default;

    // Component Overrides
    void resized() override;

private:
    // Button click handlers
    void onLoadPlugin();
    void onSavePreset();
    void onLoadPreset();

    // UI Components
    juce::TextButton loadPluginButton{"Load Plugin"};
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};

    // Prevent copy and move
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

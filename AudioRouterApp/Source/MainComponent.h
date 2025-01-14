#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioEngine.h"

/**
 * MainComponent handles the user interface and interaction logic for the application.
 */
class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override = default;

    /** Handles component resizing. */
    void resized() override;

private:
    juce::TextButton loadPluginButton{ "Load Plugin" };
    juce::TextButton savePresetButton{ "Save Preset" };
    juce::TextButton loadPresetButton{ "Load Preset" };

    AudioEngine audioEngine;

    /** Callback for loading a plugin. */
    void onLoadPlugin();

    /** Callback for saving a preset. */
    void onSavePreset();

    /** Callback for loading a preset. */
    void onLoadPreset();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

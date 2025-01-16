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
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Button click handlers
    void onLoadPlugin();
    void onSavePreset();
    void onLoadPreset();
    void setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label);
    void populateDropdown(juce::ComboBox& dropdown, const juce::StringArray& deviceNames);

    // UI Components
    juce::TextButton loadPluginButton{"Load Plugin"};
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};
    juce::AudioDeviceManager audioDeviceManager;
    juce::ComboBox hardwareInputsMenu, virtualInputsMenu, hardwareOutMenu;
    juce::Label hardwareInputsLabel, virtualInputsLabel, hardwareOutLabel;

    // Prevent copy and move
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

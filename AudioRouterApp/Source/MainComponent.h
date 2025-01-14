#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component
{
public:
    MainComponent(AudioEngine& engine);
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void populateDropdowns();
    void handleInputSelection();  // Handles user selection of input devices
    void handleOutputSelection(); // Handles user selection of output devices

    AudioEngine& audioEngine;
    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};
    juce::Label statusLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

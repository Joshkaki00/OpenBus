#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component
{
public:
    MainComponent(AudioEngine& engine);
    ~MainComponent() override;

    void resized() override;
    void paint(juce::Graphics&) override;

private:
    void populateDropdowns();
    void handleInputSelection();
    void handleOutputSelection();

    AudioEngine& audioEngine;

    // UI Components
    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};
    juce::Label statusLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

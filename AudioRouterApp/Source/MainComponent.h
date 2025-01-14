#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component {
public:
    explicit MainComponent(AudioEngine& engine); // Constructor accepting AudioEngine reference
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioEngine& audioEngine; // Reference to the audio engine

    // UI Components
    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};
    juce::Label statusLabel;

    // Helper Methods
    void populateDropdowns();
    void handleInputSelection();
    void handleOutputSelection();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

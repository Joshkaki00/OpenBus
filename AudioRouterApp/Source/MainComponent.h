#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component {
public:
    MainComponent(AudioEngine& engine);
    ~MainComponent() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void populateDropdowns();

    AudioEngine& audioEngine;

    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};
    juce::Label statusLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

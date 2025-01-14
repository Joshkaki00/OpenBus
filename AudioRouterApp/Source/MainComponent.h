#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component,
                      public juce::ComboBox::Listener
{
public:
    MainComponent(AudioEngine& engine);
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Reference to the AudioEngine
    AudioEngine& audioEngine;

    // GUI Components
    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::Label statusLabel{"Status", "Ready"};

    // Helper Methods
    void populateDropdowns();

    // Listeners
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>
#include "AudioEngine.h"

using json = nlohmann::json;

class MainComponent : public juce::Component,
                      public juce::Button::Listener,
                      public juce::ComboBox::Listener
{
public:
    MainComponent(AudioEngine& audioEngine);
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Reference to AudioEngine
    AudioEngine& audioEngine;

    // GUI Components
    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};
    juce::Label statusLabel{"Status", "Ready"};

    // Helper Methods
    void populateDropdowns();
    void savePreset();
    void loadPreset();
    void updateStatus(const juce::String& message);

    // Listeners
    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

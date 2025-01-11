#pragma once

#include <JuceHeader.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class MainComponent : public juce::Component,
                      public juce::Button::Listener,
                      public juce::ComboBox::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // GUI Components
    juce::ComboBox inputDropdown;
    juce::ComboBox outputDropdown;
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};

    // Helper Methods
    void populateDropdowns();
    void savePreset();
    void loadPreset();

    // Listeners
    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

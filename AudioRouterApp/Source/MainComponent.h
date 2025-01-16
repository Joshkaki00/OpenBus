#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component
{
public:
    // Constructor and Destructor
    MainComponent();
    ~MainComponent() override;

    // Component Overrides
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // Helper functions
    void setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label);
    void populateDropdown(juce::ComboBox& dropdown, const juce::StringArray& deviceNames);

    // UI Components
    juce::ComboBox hardwareInputsMenu, virtualInputsMenu, hardwareOutMenu;
    juce::Label hardwareInputsLabel{"Hardware Inputs"}, virtualInputsLabel{"Virtual Inputs"}, hardwareOutLabel{"Hardware Outputs"};
    juce::AudioDeviceManager audioDeviceManager;

    // Prevent copy and move
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

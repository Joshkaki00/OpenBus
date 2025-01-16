#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

// Custom LookAndFeel class to customize fonts
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        setDefaultSansSerifTypefaceName("Arial"); // Optional: Set default font family
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        return juce::Font(18.0f); // Set font size for ComboBox
    }

    juce::Font getLabelFont(juce::Label&) override
    {
        return juce::Font(18.0f); // Set font size for Label
    }
};

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
    void onLoadPlugin();
    void onSavePreset();
    void onLoadPreset();

    // UI Components
    juce::ComboBox hardwareInputsMenu, virtualInputsMenu, hardwareOutMenu;
    juce::Label hardwareInputsLabel{"Hardware Inputs"}, virtualInputsLabel{"Virtual Inputs"}, hardwareOutLabel{"Hardware Outputs"};
    juce::AudioDeviceManager audioDeviceManager;

    // Custom LookAndFeel instance
    CustomLookAndFeel customLookAndFeel;

    // Prevent copy and move
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

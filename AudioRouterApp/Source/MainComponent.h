#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

// Custom LookAndFeel class to customize fonts
class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel()
    {
        setDefaultSansSerifTypefaceName("Arial");
    }

    juce::Font getComboBoxFont(juce::ComboBox&) override
    {
        juce::FontOptions options;
        options = options.withHeight(18.0f);
        return juce::Font(options);
    }

    juce::Font getLabelFont(juce::Label&) override
    {
        juce::FontOptions options;
        options = options.withHeight(18.0f);
        return juce::Font(options);
    }
};

class MainComponent : public juce::Component,
                      public juce::ChangeListener // Use ChangeListener for device updates
{
public:
    MainComponent();
    ~MainComponent() override;

    // Component overrides
    void paint(juce::Graphics&) override;
    void resized() override;

    // ChangeListener override
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    // Helper functions
    void setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label);
    void populateDropdown(juce::ComboBox& dropdown, const juce::StringArray& items);
    void scanForPlugins();
    void populatePluginDropdown();

    // UI Components
    juce::ComboBox hardwareInputsMenu, hardwareOutMenu, pluginDropdown;
    juce::Label hardwareInputsLabel{"Hardware Inputs"}, hardwareOutLabel{"Hardware Outputs"}, pluginLabel{"Plugins"};
    juce::TextButton scanPluginsButton{"Scan Plugins"};
    juce::AudioDeviceManager audioDeviceManager;

    // Plugin scanning
    juce::StringArray scannedPlugins;

    // Custom LookAndFeel instance
    CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

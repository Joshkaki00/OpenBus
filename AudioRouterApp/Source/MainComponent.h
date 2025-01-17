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
                      public juce::ChangeListener
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
    void scanForPlugins();                         // Scan for plugins in specified directories
    void populatePluginDropdown();                 // Populate dropdown with plugins
    bool validatePlugin(const juce::File& file);   // Validate the plugin file

    // UI Components
    juce::ComboBox hardwareInputsMenu, hardwareOutMenu, pluginDropdown; // Added `pluginDropdown`
    juce::Label hardwareInputsLabel{"Hardware Inputs"},
                hardwareOutLabel{"Hardware Outputs"},
                pluginLabel{"Plugins"};            // Added `pluginLabel`
    juce::TextButton scanPluginsButton{"Scan Plugins"};
    juce::AudioDeviceManager audioDeviceManager;

    // Custom LookAndFeel
    CustomLookAndFeel customLookAndFeel;           // Added `customLookAndFeel`

    // Scanned plugins
    juce::StringArray scannedPlugins;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

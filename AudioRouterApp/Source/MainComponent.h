#pragma once

#include <JuceHeader.h>

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override = default;

    void resized() override;

private:
    void onLoadPlugin();
    void onSavePreset();
    void onLoadPreset();

    juce::TextButton loadPluginButton{"Load Plugin"};
    juce::TextButton savePresetButton{"Save Preset"};
    juce::TextButton loadPresetButton{"Load Preset"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

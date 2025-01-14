#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component,
                      public juce::Button::Listener,
                      public juce::ComboBox::Listener
{
public:
    MainComponent(AudioEngine& engine);
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    AudioEngine& audioEngine; // Reference to the audio engine

    juce::Label inputLabel{"InputLabel", "Input Device:"};
    juce::ComboBox inputDropdown;

    juce::Label outputLabel{"OutputLabel", "Output Device:"};
    juce::ComboBox outputDropdown;

    juce::TextButton refreshButton{"Refresh Devices"};
    juce::Label statusLabel{"StatusLabel", "Ready"};

    void populateDeviceDropdowns();
    void refreshDeviceList();

    void buttonClicked(juce::Button* button) override;
    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

#endif // MAINCOMPONENT_H

#include "MainComponent.h"
#include "AudioEngine.h"
#include <juce_audio_devices/juce_audio_devices.h>

MainComponent::MainComponent()
{
    // Initialize AudioDeviceManager and register as a listener
    audioDeviceManager.initialise(2, 2, nullptr, true, {}, nullptr);
    audioDeviceManager.addAudioCallback(this);

    // Setup the dropdowns
    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);

    // Populate initial input and output names
    audioDeviceListChanged();

    setSize(600, 400); // Adjust size as needed
}

MainComponent::~MainComponent()
{
    // Remove listener
    audioDeviceManager.removeAudioCallback(this);
}

void MainComponent::audioDeviceListChanged()
{
    // Get the current device and populate input/output menus
    if (auto* currentDevice = audioDeviceManager.getCurrentAudioDevice())
    {
        populateDropdown(hardwareInputsMenu, currentDevice->getInputChannelNames());
        populateDropdown(hardwareOutMenu, currentDevice->getOutputChannelNames());
    }
}

void MainComponent::populateDropdown(juce::ComboBox& dropdown, const juce::StringArray& items)
{
    dropdown.clear();
    for (int i = 0; i < items.size(); ++i)
        dropdown.addItem(items[i], i + 1);

    if (items.isEmpty())
        dropdown.addItem("No devices available", 1);
}

void MainComponent::setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label)
{
    addAndMakeVisible(dropdown);
    dropdown.setJustificationType(juce::Justification::centredLeft);

    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&dropdown, true);
}

#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    // Initialize AudioDeviceManager
    audioDeviceManager.initialise(2, 2, nullptr, true, {}, nullptr);

    // Register MainComponent as a listener
    audioDeviceManager.addChangeListener(this);

    // Setup dropdowns
    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);

    // Populate dropdowns with initial device names
    changeListenerCallback(&audioDeviceManager);

    setSize(600, 400);
}

MainComponent::~MainComponent()
{
    // Remove MainComponent as a listener
    audioDeviceManager.removeChangeListener(this);
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &audioDeviceManager)
    {
        // Get the current device and update dropdowns
        if (auto* currentDevice = audioDeviceManager.getCurrentAudioDevice())
        {
            populateDropdown(hardwareInputsMenu, currentDevice->getInputChannelNames());
            populateDropdown(hardwareOutMenu, currentDevice->getOutputChannelNames());
        }
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

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto labelHeight = 20;
    auto dropdownHeight = 30;
    auto spacing = 10;

    hardwareInputsLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareInputsMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(spacing);

    hardwareOutLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareOutMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
}

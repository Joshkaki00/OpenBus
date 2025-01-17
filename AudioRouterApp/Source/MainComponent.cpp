#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    // Initialize LookAndFeel
    setLookAndFeel(&customLookAndFeel);

    // Initialize AudioDeviceManager
    audioDeviceManager.initialise(2, 2, nullptr, true, {}, nullptr);

    // Register MainComponent as a listener
    audioDeviceManager.addChangeListener(this);

    // Setup dropdowns
    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);
    setupDropdown(pluginDropdown, "Plugins", pluginLabel);

    // Add the Scan Plugins button
    addAndMakeVisible(scanPluginsButton);
    scanPluginsButton.onClick = [this]() { scanForPlugins(); };

    // Populate dropdowns with initial device names
    changeListenerCallback(&audioDeviceManager);

    setSize(600, 500);
}

MainComponent::~MainComponent()
{
    // Remove MainComponent as a listener
    audioDeviceManager.removeChangeListener(this);

    // Reset LookAndFeel
    setLookAndFeel(nullptr);
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

void MainComponent::scanForPlugins()
{
    scannedPlugins.clear(); // Clear previous results

    // List of directories to scan
    juce::Array<juce::File> pluginDirectories = {
        juce::File("~/Library/Audio/Plug-Ins/VST"),
        juce::File("~/Library/Audio/Plug-Ins/VST3"),
        juce::File("~/Library/Audio/Plug-Ins/Components"),
        juce::File("/Library/Audio/Plug-Ins/VST"),
        juce::File("/Library/Audio/Plug-Ins/VST3"),
        juce::File("/Library/Audio/Plug-Ins/Components")
    };

    // Supported extensions
    juce::StringArray supportedExtensions = { ".vst", ".vst3", ".component" };

    for (const auto& dir : pluginDirectories)
    {
        if (dir.isDirectory())
        {
            auto files = dir.findChildFiles(juce::File::findFiles, true);
            for (const auto& file : files)
            {
                if (supportedExtensions.contains(file.getFileExtension()))
                {
                    scannedPlugins.add(file.getFullPathName());
                }
            }
        }
    }

    // Update the plugin dropdown
    populatePluginDropdown();
}

void MainComponent::populatePluginDropdown()
{
    pluginDropdown.clear();

    if (scannedPlugins.isEmpty())
    {
        pluginDropdown.addItem("No plugins found", 1);
    }
    else
    {
        for (int i = 0; i < scannedPlugins.size(); ++i)
        {
            pluginDropdown.addItem(scannedPlugins[i], i + 1);
        }
    }
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
    auto buttonHeight = 40;
    auto spacing = 10;

    hardwareInputsLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareInputsMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(spacing);

    hardwareOutLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareOutMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(spacing);

    pluginLabel.setBounds(area.removeFromTop(labelHeight));
    pluginDropdown.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(spacing);

    scanPluginsButton.setBounds(area.removeFromTop(buttonHeight).reduced(0, 5));
}

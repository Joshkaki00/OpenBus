#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    // Initialize LookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();
    setLookAndFeel(customLookAndFeel.get());

    // Setup hardware input/output dropdowns
    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);

    // Populate hardware inputs and outputs
    if (auto* currentDevice = audioDeviceManager.getCurrentAudioDevice())
    {
        populateDropdown(hardwareInputsMenu, currentDevice->getInputChannelNames());
        populateDropdown(hardwareOutMenu, currentDevice->getOutputChannelNames());
    }

    // Setup plugin list dropdown
    addAndMakeVisible(pluginListMenu);
    pluginListLabel.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(pluginListLabel);

    // Setup scan plugins button
    addAndMakeVisible(scanPluginsButton);
    scanPluginsButton.onClick = [this]() { scanForPlugins(); };

    setSize(600, 400);
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto labelHeight = 20;
    auto dropdownHeight = 40;
    auto buttonHeight = 30;
    auto verticalSpacing = 10;

    // Layout hardware inputs
    hardwareInputsLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareInputsMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(verticalSpacing);

    // Layout hardware outputs
    hardwareOutLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareOutMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(verticalSpacing);

    // Layout plugin list
    pluginListLabel.setBounds(area.removeFromTop(labelHeight));
    pluginListMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    area.removeFromTop(verticalSpacing);

    // Layout scan button
    scanPluginsButton.setBounds(area.removeFromTop(buttonHeight).reduced(0, 5));
}

void MainComponent::setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label)
{
    addAndMakeVisible(dropdown);
    label.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
}

void MainComponent::populateDropdown(juce::ComboBox& dropdown, const juce::StringArray& deviceNames)
{
    for (const auto& name : deviceNames)
    {
        dropdown.addItem(name, dropdown.getNumItems() + 1);
    }
}

void MainComponent::scanForPlugins()
{
    scannedPlugins.clear();

    // Directories to scan
    juce::Array<juce::File> pluginDirectories = {
        juce::File("~/Library/Audio/Plug-Ins/VST"),
        juce::File("~/Library/Audio/Plug-Ins/VST3"),
        juce::File("~/Library/Audio/Plug-Ins/Components"),
        juce::File("/Library/Audio/Plug-Ins/VST"),
        juce::File("/Library/Audio/Plug-Ins/VST3"),
        juce::File("/Library/Audio/Plug-Ins/Components")
    };

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
                    scannedPlugins.add(file.getFileName());
                }
            }
        }
    }

    populateDropdownWithPlugins();
}

void MainComponent::populateDropdownWithPlugins()
{
    pluginListMenu.clear();

    if (scannedPlugins.isEmpty())
    {
        pluginListMenu.addItem("No plugins found", 1);
    }
    else
    {
        for (int i = 0; i < scannedPlugins.size(); ++i)
        {
            pluginListMenu.addItem(scannedPlugins[i], i + 1);
        }
    }
}

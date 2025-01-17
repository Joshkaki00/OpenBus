#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    // Initialize the LookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();
    setLookAndFeel(customLookAndFeel.get());

    // Setup the hardware dropdowns
    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(virtualInputsMenu, "Virtual Inputs", virtualInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);

    // Populate hardware inputs
    juce::StringArray hardwareInputs = { "Mic 1", "Mic 2", "Line In" }; // Example list
    populateDropdown(hardwareInputsMenu, hardwareInputs);

    // Populate virtual inputs
    juce::StringArray virtualInputs = { "Virtual Input 1", "Virtual Input 2" }; // Example list
    populateDropdown(virtualInputsMenu, virtualInputs);

    // Populate hardware outputs
    juce::StringArray hardwareOutputs = { "Speakers", "Headphones", "Line Out" }; // Example list
    populateDropdown(hardwareOutMenu, hardwareOutputs);

    // Setup the plugin list dropdown
    addAndMakeVisible(pluginListMenu);
    pluginListLabel.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(pluginListLabel);

    // Setup the scan plugins button
    addAndMakeVisible(scanPluginsButton);
    scanPluginsButton.onClick = [this]() { scanForPlugins(); };

    setSize(600, 500); // Increased window size
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey); // Background color
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

    // Layout virtual inputs
    virtualInputsLabel.setBounds(area.removeFromTop(labelHeight));
    virtualInputsMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
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
    label.setJustificationType(juce::Justification::centredLeft);
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

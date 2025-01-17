#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    // Initialize the LookAndFeel
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();
    setLookAndFeel(customLookAndFeel.get());

    // Setup the plugin list dropdown
    addAndMakeVisible(pluginListMenu);
    pluginListLabel.setJustificationType(juce::Justification::centredTop);
    addAndMakeVisible(pluginListLabel);

    // Setup the scan plugins button
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
    g.fillAll(juce::Colours::lightgrey); // Background color
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto labelHeight = 20;
    auto dropdownHeight = 40;
    auto buttonHeight = 30;

    // Layout the components
    pluginListLabel.setBounds(area.removeFromTop(labelHeight));
    pluginListMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 5));
    scanPluginsButton.setBounds(area.removeFromTop(buttonHeight).reduced(0, 5));
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
                    scannedPlugins.add(file.getFullPathName());
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

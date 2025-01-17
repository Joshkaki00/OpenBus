#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    setLookAndFeel(&customLookAndFeel);
    audioDeviceManager.initialiseWithDefaultDevices(2, 2);
    audioDeviceManager.addChangeListener(this);

    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);
    setupDropdown(pluginDropdown, "Plugins", pluginLabel);

    addAndMakeVisible(scanPluginsButton);
    scanPluginsButton.onClick = [this]() { scanForPlugins(); };

    changeListenerCallback(&audioDeviceManager);
    setSize(600, 500);
}

MainComponent::~MainComponent()
{
    audioDeviceManager.removeChangeListener(this);
    setLookAndFeel(nullptr);
}

void MainComponent::setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label)
{
    addAndMakeVisible(dropdown);
    dropdown.setJustificationType(juce::Justification::centredLeft);

    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&dropdown, true);
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &audioDeviceManager)
    {
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

void MainComponent::scanForPlugins()
{
    scannedPlugins.clear();

    juce::Array<juce::File> pluginDirectories = {
        juce::File("/Library/Audio/Plug-Ins/VST"),
        juce::File("/Library/Audio/Plug-Ins/VST3"),
        juce::File("/Library/Audio/Plug-Ins/Components"),
        juce::File("~/Library/Audio/Plug-Ins/VST"), // User directory
        juce::File("~/Library/Audio/Plug-Ins/VST3"),
        juce::File("~/Library/Audio/Plug-Ins/Components"),
        juce::File("~/.vst"),
        juce::File("/usr/lib/vst"),
        juce::File("/usr/local/lib/vst")
    };

    juce::StringArray supportedExtensions = { ".vst", ".vst3", ".component" };

    for (const auto& dir : pluginDirectories)
    {
        DBG("Scanning directory: " << dir.getFullPathName());

        if (!dir.isDirectory())
        {
            DBG("Directory not found: " << dir.getFullPathName());
            continue;
        }

        auto files = dir.findChildFiles(juce::File::findDirectories, true); // Look for directories (plugin bundles)
        DBG("Number of potential plugin bundles found: " << files.size());

        for (const auto& file : files)
        {
            if (supportedExtensions.contains(file.getFileExtension()))
            {
                // Check if this is a plugin bundle
                auto binary = file.getChildFile("Contents/MacOS").getChildFile(file.getFileNameWithoutExtension());
                if (binary.existsAsFile())
                {
                    DBG("Found plugin binary: " << binary.getFullPathName());
                    if (validatePlugin(binary))
                    {
                        scannedPlugins.add(file.getFullPathName());
                    }
                    else
                    {
                        DBG("Invalid plugin binary: " << binary.getFullPathName());
                    }
                }
                else
                {
                    DBG("No binary found in plugin bundle: " << file.getFullPathName());
                }
            }
        }
    }

    populatePluginDropdown();
}

bool MainComponent::validatePlugin(const juce::File& file)
{
    juce::AudioPluginFormatManager formatManager;
    formatManager.addDefaultFormats();

    juce::OwnedArray<juce::PluginDescription> pluginDescriptions;

    for (auto* format : formatManager.getFormats())
    {
        if (format->fileMightContainThisPluginType(file.getFullPathName()))
        {
            format->findAllTypesForFile(pluginDescriptions, file.getFullPathName());
        }
    }

    if (pluginDescriptions.isEmpty())
    {
        DBG("No valid plugin descriptions found for: " << file.getFullPathName());
        return false;
    }

    DBG("Valid plugin found: " << pluginDescriptions[0]->name);
    return true;
}

void MainComponent::populatePluginDropdown()
{
    pluginDropdown.clear();

    if (scannedPlugins.isEmpty())
    {
        DBG("No plugins found.");
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

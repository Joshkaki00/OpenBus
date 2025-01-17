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
    DBG("Starting plugin scan...");

    juce::AudioPluginFormatManager formatManager;
    formatManager.addDefaultFormats();

    DBG("Registered plugin formats:");
    for (int i = 0; i < formatManager.getNumFormats(); ++i)
    {
        DBG(" - " << formatManager.getFormat(i)->getName());
    }

    juce::Array<juce::File> pluginDirectories = {
        juce::File("/Library/Audio/Plug-Ins/VST"),          // macOS
        juce::File("/Library/Audio/Plug-Ins/VST3"),         // macOS
        juce::File("/Library/Audio/Plug-Ins/Components"),   // macOS
        juce::File("~/.vst"),                               // Linux
        juce::File("/usr/lib/vst"),                         // Linux
        juce::File("/usr/local/lib/vst")                    // Linux
    };

    for (const auto& dir : pluginDirectories)
    {
        if (!dir.isDirectory())
        {
            DBG("Directory not found: " << dir.getFullPathName());
            continue;
        }

        DBG("Scanning directory: " << dir.getFullPathName());
        auto files = dir.findChildFiles(juce::File::findFiles, false); // Non-recursive

        for (const auto& file : files)
        {
            if (file.hasFileExtension({ ".vst", ".vst3", ".component" }))
            {
                DBG("Found plugin bundle: " << file.getFullPathName());

                if (validatePlugin(file))
                {
                    scannedPlugins.add(file.getFullPathName());
                    DBG("Plugin added to list: " << file.getFullPathName());
                }
                else
                {
                    DBG("Invalid plugin: " << file.getFullPathName());
                }
            }
            else
            {
                DBG("Skipping unsupported file: " << file.getFullPathName());
            }
        }
    }

    populatePluginDropdown();
    DBG("Plugin scan completed.");
}

bool MainComponent::validatePlugin(const juce::File& file)
{
    juce::AudioPluginFormatManager formatManager;
    formatManager.addDefaultFormats();

    DBG("Validating plugin binary: " << file.getFullPathName());

    juce::OwnedArray<juce::PluginDescription> pluginDescriptions;

    for (auto* format : formatManager.getFormats())
    {
        DBG("Trying format: " << format->getName());

        if (format->fileMightContainThisPluginType(file.getFullPathName()))
        {
            try
            {
                DBG("Checking if file might contain a valid plugin: " << file.getFullPathName());
                format->findAllTypesForFile(pluginDescriptions, file.getFullPathName());

                if (!pluginDescriptions.isEmpty())
                {
                    DBG("Plugin is valid: " << pluginDescriptions[0]->name);
                    return true;
                }
                else
                {
                    DBG("No valid plugin descriptions found for: " << file.getFullPathName());
                }
            }
            catch (const std::exception& e)
            {
                DBG("Validation error for " << file.getFullPathName() << ": " << e.what());
            }
            catch (...)
            {
                DBG("Unknown validation error for: " << file.getFullPathName());
            }
        }
        else
        {
            DBG("File does not match format: " << format->getName());
        }
    }

    DBG("Validation failed for plugin binary: " << file.getFullPathName());
    return false;
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

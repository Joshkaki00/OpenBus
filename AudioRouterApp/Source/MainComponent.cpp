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
    juce::StringArray pluginDirectories;

    // Define default plugin directories based on the OS
    #if JUCE_MAC
    pluginDirectories.add("/Library/Audio/Plug-Ins/VST");
    pluginDirectories.add("/Library/Audio/Plug-Ins/VST3");
    pluginDirectories.add("/Library/Audio/Plug-Ins/Components"); // AU plugins
    pluginDirectories.add("~/Library/Audio/Plug-Ins/VST");
    pluginDirectories.add("~/Library/Audio/Plug-Ins/VST3");
    pluginDirectories.add("~/Library/Audio/Plug-Ins/Components"); // User-level AU plugins
    #elif JUCE_LINUX
    pluginDirectories.add("~/.vst");
    pluginDirectories.add("~/.vst3");
    pluginDirectories.add("/usr/lib/vst");
    pluginDirectories.add("/usr/lib/vst3");
    pluginDirectories.add("/usr/local/lib/vst");
    pluginDirectories.add("/usr/local/lib/vst3");
    #endif

    DBG("Starting plugin scan...");

    // Add formats to the AudioPluginFormatManager
    juce::AudioPluginFormatManager pluginFormatManager;
    pluginFormatManager.addDefaultFormats(); // Adds AU, VST, and VST3 formats by default

    // List of valid file extensions
    juce::StringArray validExtensions { "vst", "vst3", "component" };

    for (const auto& directoryPath : pluginDirectories)
    {
        juce::File directory(directoryPath);

        if (!directory.isDirectory())
        {
            DBG("Directory not found: " << directory.getFullPathName());
            continue;
        }

        DBG("Scanning directory: " << directory.getFullPathName());

        // Use RangedDirectoryIterator for better and modern file iteration
        for (const auto& file : juce::RangedDirectoryIterator(directory, false, "*.*"))
        {
            DBG("Scanning file: " << file.getFile().getFileName());

            // Check for plugin validity using extensions
            if (file.getFile().hasFileExtension(validExtensions))
            {
                DBG("Found potential plugin: " << file.getFile().getFullPathName());

                juce::PluginDescription description;
                description.fileOrIdentifier = file.getFile().getFullPathName();
                description.name = file.getFile().getFileName();
                description.pluginFormatName = "Unknown";

                juce::String errorMessage;

                auto plugin = pluginFormatManager.createPluginInstance(
                    description,
                    44100.0, // sample rate
                    512,     // buffer size
                    errorMessage
                );

                if (plugin)
                {
                    DBG("Successfully loaded plugin: " << file.getFile().getFileName());
                    scannedPlugins.add(file.getFile().getFullPathName());
                }
                else
                {
                    DBG("Failed to load plugin: " << file.getFile().getFileName() << " - " << errorMessage);
                }
            }
            else
            {
                DBG("Skipping unsupported file: " << file.getFile().getFullPathName());
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

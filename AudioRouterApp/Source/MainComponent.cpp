#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    // Initialize the audio device manager with default devices
    audioDeviceManager.initialiseWithDefaultDevices(2, 2); // 2 inputs, 2 outputs

    // Setup the dropdowns
    setupDropdown(hardwareInputsMenu, "Hardware Inputs", hardwareInputsLabel);
    setupDropdown(virtualInputsMenu, "Virtual Inputs", virtualInputsLabel);
    setupDropdown(hardwareOutMenu, "Hardware Outputs", hardwareOutLabel);

    // Get available device types
    auto availableDeviceTypes = audioDeviceManager.getAvailableDeviceTypes();

    // Populate hardware inputs
    for (auto* deviceType : availableDeviceTypes)
    {
        auto deviceNames = deviceType->getDeviceNames(0); // Get input device names
        populateDropdown(hardwareInputsMenu, deviceNames);
    }

    // Populate virtual inputs (placeholder names) and hardware outputs
    populateDropdown(virtualInputsMenu, { "Virtual Input 1", "Virtual Input 2" });
    if (auto* currentDevice = audioDeviceManager.getCurrentAudioDevice())
        populateDropdown(hardwareOutMenu, currentDevice->getOutputChannelNames());

    // Add listeners to dropdowns
    hardwareInputsMenu.onChange = [&]() {
        auto selectedInput = hardwareInputsMenu.getText();
        DBG("Selected Hardware Input: " << selectedInput);
    };

    virtualInputsMenu.onChange = [&]() {
        auto selectedInput = virtualInputsMenu.getText();
        DBG("Selected Virtual Input: " << selectedInput);
    };

    hardwareOutMenu.onChange = [&]() {
        auto selectedOutput = hardwareOutMenu.getText();
        DBG("Selected Hardware Output: " << selectedOutput);
    };

    setSize(400, 300);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey); // Background color
}

void MainComponent::resized()
{
    // Define bounds for layout
    auto area = getLocalBounds().reduced(20);
    auto labelHeight = 20;
    auto dropdownHeight = 30;

    hardwareInputsLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareInputsMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 10));

    virtualInputsLabel.setBounds(area.removeFromTop(labelHeight));
    virtualInputsMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 10));

    hardwareOutLabel.setBounds(area.removeFromTop(labelHeight));
    hardwareOutMenu.setBounds(area.removeFromTop(dropdownHeight).reduced(0, 10));
}

void MainComponent::setupDropdown(juce::ComboBox& dropdown, const juce::String& labelText, juce::Label& label)
{
    addAndMakeVisible(dropdown);
    dropdown.setJustificationType(juce::Justification::centredLeft);

    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&dropdown, true);
}

void MainComponent::populateDropdown(juce::ComboBox& dropdown, const juce::StringArray& deviceNames)
{
    for (const auto& name : deviceNames)
    {
        dropdown.addItem(name, dropdown.getNumItems() + 1);
    }
}

void MainComponent::onLoadPlugin()
{
    juce::FileChooser chooser("Select a plugin to load...", {}, "*.vst3;*.vst;*.component");
    chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                if (!AudioEngine::getInstance().loadPlugin(file))
                {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                        "Error",
                        "Failed to load plugin.");
                }
            }
        });
}

void MainComponent::onSavePreset()
{
    juce::FileChooser chooser("Save Preset...", {}, "*.preset");
    chooser.launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                if (!AudioEngine::getInstance().savePreset(file))
                {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                        "Error",
                        "Failed to save preset.");
                }
            }
        });
}

void MainComponent::onLoadPreset()
{
    juce::FileChooser chooser("Load Preset...", {}, "*.preset");
    chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
        {
            auto file = fc.getResult();
            if (file.existsAsFile())
            {
                if (!AudioEngine::getInstance().loadPreset(file))
                {
                    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                        "Error",
                        "Failed to load preset.");
                }
            }
        });
}

#include "MainComponent.h"

MainComponent::MainComponent(AudioEngine& engine)
    : audioEngine(engine)
{
    // Add UI components to the interface
    addAndMakeVisible(inputDropdown);
    addAndMakeVisible(outputDropdown);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);
    addAndMakeVisible(statusLabel);

    // Set up listeners and actions
    inputDropdown.onChange = [this] { handleInputSelection(); };
    outputDropdown.onChange = [this] { handleOutputSelection(); };
    
    savePresetButton.onClick = [this] {
        auto response = audioEngine.savePreset("DefaultPreset");
        statusLabel.setText(juce::String(response.dump()), juce::dontSendNotification);
    };

    loadPresetButton.onClick = [this] {
        auto response = audioEngine.loadPreset("DefaultPreset");
        statusLabel.setText(juce::String(response.dump()), juce::dontSendNotification);
    };

    populateDropdowns();
    setSize(400, 300);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey); // Background color
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawFittedText("Audio Router", getLocalBounds(), juce::Justification::centred, 1);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto rowHeight = 40;

    inputDropdown.setBounds(area.removeFromTop(rowHeight));
    outputDropdown.setBounds(area.removeFromTop(rowHeight));
    area.removeFromTop(20);
    savePresetButton.setBounds(area.removeFromLeft(area.getWidth() / 2).reduced(5));
    loadPresetButton.setBounds(area.reduced(5));
    statusLabel.setBounds(area);
}

void MainComponent::populateDropdowns()
{
    auto deviceList = audioEngine.getDeviceList();
    if (deviceList["status"] == "success")
    {
        inputDropdown.clear();
        outputDropdown.clear();

        for (const auto& input : deviceList["inputs"])
            inputDropdown.addItem(juce::String(input.get<std::string>()), inputDropdown.getNumItems() + 1);

        for (const auto& output : deviceList["outputs"])
            outputDropdown.addItem(juce::String(output.get<std::string>()), outputDropdown.getNumItems() + 1);
    }
    else
    {
        statusLabel.setText("Error retrieving devices", juce::dontSendNotification);
    }
}

void MainComponent::handleInputSelection()
{
    int selectedItemIndex = inputDropdown.getSelectedId() - 1; // Dropdown IDs are 1-based
    if (selectedItemIndex >= 0)
    {
        auto response = audioEngine.setInputDevice(inputDropdown.getItemText(selectedItemIndex).toStdString());
        statusLabel.setText(juce::String(response.dump()), juce::dontSendNotification);
    }
}

void MainComponent::handleOutputSelection()
{
    int selectedItemIndex = outputDropdown.getSelectedId() - 1; // Dropdown IDs are 1-based
    if (selectedItemIndex >= 0)
    {
        auto response = audioEngine.setOutputDevice(outputDropdown.getItemText(selectedItemIndex).toStdString());
        statusLabel.setText(juce::String(response.dump()), juce::dontSendNotification);
    }
}

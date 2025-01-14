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
        // Handle saving presets - ensure AudioEngine has savePreset method
        audioEngine.savePreset("DefaultPreset");
        statusLabel.setText("Preset saved", juce::dontSendNotification);
    };

    loadPresetButton.onClick = [this] {
        // Handle loading presets - ensure AudioEngine has loadPreset method
        audioEngine.loadPreset("DefaultPreset");
        statusLabel.setText("Preset loaded", juce::dontSendNotification);
    };

    // Initialize dropdowns and UI
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
    // Populate available input and output devices
    auto devices = audioEngine.getDeviceList();
    auto inputs = devices["inputs"];
    auto outputs = devices["outputs"];

    for (const auto& input : inputs) {
        inputDropdown.addItem(input.get<std::string>(), inputDropdown.getNumItems() + 1);
    }

    for (const auto& output : outputs) {
        outputDropdown.addItem(output.get<std::string>(), outputDropdown.getNumItems() + 1);
    }

    inputDropdown.setSelectedId(1);
    outputDropdown.setSelectedId(1);
}

void MainComponent::handleInputSelection()
{
    auto selectedInput = inputDropdown.getText().toStdString();
    audioEngine.setInputDevice(selectedInput);
    statusLabel.setText("Selected Input: " + selectedInput, juce::dontSendNotification);
}

void MainComponent::handleOutputSelection()
{
    auto selectedOutput = outputDropdown.getText().toStdString();
    audioEngine.setOutputDevice(selectedOutput);
    statusLabel.setText("Selected Output: " + selectedOutput, juce::dontSendNotification);
}

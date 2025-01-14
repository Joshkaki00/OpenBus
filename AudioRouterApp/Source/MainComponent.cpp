#include "MainComponent.h"

MainComponent::MainComponent(AudioEngine& engine) : audioEngine(engine) {
    // Add UI components to the interface
    addAndMakeVisible(inputDropdown);
    addAndMakeVisible(outputDropdown);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);
    addAndMakeVisible(statusLabel);

    // Set up listeners and actions
    inputDropdown.onChange = [this] { handleInputSelection(); };
    outputDropdown.onChange = [this] { handleOutputSelection(); };
    savePresetButton.onClick = [this] { audioEngine.savePreset("DefaultPreset"); };
    loadPresetButton.onClick = [this] { audioEngine.loadPreset("DefaultPreset"); };

    // Initialize dropdowns and UI
    populateDropdowns();
    setSize(400, 300);
}

MainComponent::~MainComponent() {
    // Clean up listeners
}

void MainComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::darkgrey); // Background color
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawFittedText("Audio Router", getLocalBounds(), juce::Justification::centred, 1);
}

void MainComponent::resized() {
    auto area = getLocalBounds().reduced(20);
    auto rowHeight = 40;

    inputDropdown.setBounds(area.removeFromTop(rowHeight));
    area.removeFromTop(10);
    outputDropdown.setBounds(area.removeFromTop(rowHeight));
    area.removeFromTop(20);
    savePresetButton.setBounds(area.removeFromTop(rowHeight).removeFromLeft(area.getWidth() / 2).reduced(5));
    loadPresetButton.setBounds(area.removeFromTop(rowHeight).reduced(5));
    area.removeFromTop(10);
    statusLabel.setBounds(area);
}

void MainComponent::populateDropdowns() {
    // Populate available input and output devices
    auto devices = audioEngine.getDeviceList();
    auto inputs = devices["inputs"];
    auto outputs = devices["outputs"];

    for (const auto& input : inputs) {
        juce::String inputName = juce::String(input.get<std::string>()); // Convert to std::string and then to juce::String
        inputDropdown.addItem(inputName, inputDropdown.getNumItems() + 1);
    }

    for (const auto& output : outputs) {
        juce::String outputName = juce::String(output.get<std::string>()); // Convert to std::string and then to juce::String
        outputDropdown.addItem(outputName, outputDropdown.getNumItems() + 1);
    }

    inputDropdown.setSelectedId(1);
    outputDropdown.setSelectedId(1);
}

void MainComponent::handleInputSelection() {
    auto selectedInput = inputDropdown.getText();
    audioEngine.setInputDevice(selectedInput.toStdString());
    statusLabel.setText("Selected Input: " + selectedInput, juce::dontSendNotification);
}

void MainComponent::handleOutputSelection() {
    auto selectedOutput = outputDropdown.getText();
    audioEngine.setOutputDevice(selectedOutput.toStdString());
    statusLabel.setText("Selected Output: " + selectedOutput, juce::dontSendNotification);
}

#include "MainComponent.h"

MainComponent::MainComponent(AudioEngine& engine) : audioEngine(engine) {
    // Add UI components to the interface
    addAndMakeVisible(inputDropdown);
    addAndMakeVisible(outputDropdown);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);
    addAndMakeVisible(statusLabel);

    // Set up listeners and actions
    inputDropdown.onChange = [this] { audioEngine.setInputDevice(inputDropdown.getText().toStdString()); };
    outputDropdown.onChange = [this] { audioEngine.setOutputDevice(outputDropdown.getText().toStdString()); };

    savePresetButton.onClick = [this] {
        auto result = audioEngine.savePreset("DefaultPreset");
        statusLabel.setText(result["message"].get<std::string>(), juce::dontSendNotification);
    };

    loadPresetButton.onClick = [this] {
        auto result = audioEngine.loadPreset("DefaultPreset");
        statusLabel.setText(result["message"].get<std::string>(), juce::dontSendNotification);
    };

    // Initialize dropdowns and UI
    populateDropdowns();
    setSize(400, 300);
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
    outputDropdown.setBounds(area.removeFromTop(rowHeight));
    savePresetButton.setBounds(area.removeFromTop(rowHeight).removeFromLeft(area.getWidth() / 2).reduced(5));
    loadPresetButton.setBounds(area.removeFromTop(rowHeight).reduced(5));
    statusLabel.setBounds(area);
}

void MainComponent::populateDropdowns() {
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

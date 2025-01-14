#include "MainComponent.h"

MainComponent::MainComponent(AudioEngine& engine) : audioEngine(engine)
{
    addAndMakeVisible(inputDropdown);
    addAndMakeVisible(outputDropdown);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);
    addAndMakeVisible(statusLabel);

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

#include "MainComponent.h"

// Constructor
MainComponent::MainComponent(AudioEngine& engine) : audioEngine(engine)
{
    // Add and configure components
    addAndMakeVisible(inputDropdown);
    addAndMakeVisible(outputDropdown);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);
    addAndMakeVisible(statusLabel);

    inputDropdown.addListener(this);
    outputDropdown.addListener(this);
    savePresetButton.addListener(this);
    loadPresetButton.addListener(this);

    populateDropdowns();

    setSize(400, 300);
}

// Destructor
MainComponent::~MainComponent()
{
    inputDropdown.removeListener(this);
    outputDropdown.removeListener(this);
    savePresetButton.removeListener(this);
    loadPresetButton.removeListener(this);
}

// GUI Painting
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);
    g.drawFittedText("Audio Router", getLocalBounds().reduced(20), juce::Justification::centred, 1);
}

// Resize Components
void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    auto rowHeight = 40;

    inputDropdown.setBounds(area.removeFromTop(rowHeight));
    area.removeFromTop(10);
    outputDropdown.setBounds(area.removeFromTop(rowHeight));
    area.removeFromTop(20);
    savePresetButton.setBounds(area.removeFromTop(rowHeight).removeFromLeft(area.getWidth() / 2).reduced(5));
    loadPresetButton.setBounds(area.removeFromTop(rowHeight).reduced(5));
    statusLabel.setBounds(area.removeFromTop(30));
}

// Populate Dropdowns
void MainComponent::populateDropdowns()
{
    inputDropdown.addItem("Input 1", 1);
    inputDropdown.addItem("Input 2", 2);
    inputDropdown.addItem("Input 3", 3);

    outputDropdown.addItem("Output 1", 1);
    outputDropdown.addItem("Output 2", 2);
    outputDropdown.addItem("Output 3", 3);

    inputDropdown.setSelectedId(1);
    outputDropdown.setSelectedId(1);
}

// Save Preset
void MainComponent::savePreset()
{
    auto presetPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("preset.json");

    json preset = {
        {"input", inputDropdown.getText().toStdString()},
        {"output", outputDropdown.getText().toStdString()}
    };

    try
    {
        presetPath.replaceWithText(preset.dump(4)); // Save JSON with proper formatting
        updateStatus("Preset saved successfully.");
    }
    catch (const std::exception& e)
    {
        updateStatus("Failed to save preset: " + juce::String(e.what()));
    }
}

// Load Preset
void MainComponent::loadPreset()
{
    auto presetPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("preset.json");

    if (!presetPath.existsAsFile())
    {
        updateStatus("Preset file not found!");
        return;
    }

    try
    {
        auto jsonText = presetPath.loadFileAsString().toStdString();
        auto preset = json::parse(jsonText);

        if (preset.contains("input") && preset.contains("output"))
        {
            inputDropdown.setText(juce::String(preset["input"].get<std::string>()), juce::dontSendNotification);
            outputDropdown.setText(juce::String(preset["output"].get<std::string>()), juce::dontSendNotification);
            updateStatus("Preset loaded successfully.");
        }
        else
        {
            throw std::runtime_error("Invalid preset structure.");
        }
    }
    catch (const std::exception& e)
    {
        updateStatus("Failed to load preset: " + juce::String(e.what()));
    }
}

// Update Status Label
void MainComponent::updateStatus(const juce::String& message)
{
    statusLabel.setText(message, juce::dontSendNotification);
}

// Button Click Handler
void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &savePresetButton)
    {
        savePreset();
    }
    else if (button == &loadPresetButton)
    {
        loadPreset();
    }
}

// ComboBox Change Handler
void MainComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &inputDropdown)
    {
        DBG("Input selected: " << inputDropdown.getText());
    }
    else if (comboBoxThatHasChanged == &outputDropdown)
    {
        DBG("Output selected: " << outputDropdown.getText());
    }
}

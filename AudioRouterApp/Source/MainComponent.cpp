#include "MainComponent.h"

// Constructor
MainComponent::MainComponent()
{
    // Add and configure components
    addAndMakeVisible(inputDropdown);
    addAndMakeVisible(outputDropdown);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);

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
    g.drawFittedText("Audio Router", getLocalBounds(), juce::Justification::centred, 1);
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
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Preset Saved", "Preset saved successfully.");
    }
    catch (const std::exception& e)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Failed to save preset: " + juce::String(e.what()));
    }
}

// Load Preset
void MainComponent::loadPreset()
{
    auto presetPath = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("preset.json");

    if (!presetPath.existsAsFile())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Preset file not found!");
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

            juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Preset Loaded", "Preset loaded successfully.");
        }
        else
        {
            throw std::runtime_error("Invalid preset structure.");
        }
    }
    catch (const std::exception& e)
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Failed to load preset: " + juce::String(e.what()));
    }
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

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

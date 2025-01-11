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


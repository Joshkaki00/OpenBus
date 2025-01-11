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


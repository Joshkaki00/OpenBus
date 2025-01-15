#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(loadPluginButton);
    loadPluginButton.onClick = [this] { onLoadPlugin(); };

    addAndMakeVisible(savePresetButton);
    savePresetButton.onClick = [this] { onSavePreset(); };

    addAndMakeVisible(loadPresetButton);
    loadPresetButton.onClick = [this] { onLoadPreset(); };
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Audio Router App", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto buttonHeight = 40;

    loadPluginButton.setBounds(area.removeFromTop(buttonHeight).reduced(5));
    savePresetButton.setBounds(area.removeFromTop(buttonHeight).reduced(5));
    loadPresetButton.setBounds(area.removeFromTop(buttonHeight).reduced(5));
}

void MainComponent::onLoadPlugin()
{
    juce::FileChooser chooser("Select a plugin to load...", {}, "*.vst3;*.vst;*.component");
    chooser.launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file.exists())
        {
            auto result = audioEngine.loadPlugin(file);
            if (result["status"] != "success")
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Failed to load plugin.");
            }
        }
    });
}

void MainComponent::onSavePreset()
{
    juce::FileChooser chooser("Save preset file...", {}, "*.preset");
    chooser.launchAsync(juce::FileBrowserComponent::saveMode, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (!file.getFullPathName().isEmpty())
        {
            auto result = audioEngine.savePreset(file);
            if (result["status"] != "success")
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Failed to save preset.");
            }
        }
    });
}

void MainComponent::onLoadPreset()
{
    juce::FileChooser chooser("Select a preset file to load...", {}, "*.preset");
    chooser.launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file.exists())
        {
            auto result = audioEngine.loadPreset(file);
            if (result["status"] != "success")
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "Error", "Failed to load preset.");
            }
        }
    });
}

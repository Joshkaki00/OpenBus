#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    loadPluginButton.onClick = [this]() { onLoadPlugin(); };
    savePresetButton.onClick = [this]() { onSavePreset(); };
    loadPresetButton.onClick = [this]() { onLoadPreset(); };

    addAndMakeVisible(loadPluginButton);
    addAndMakeVisible(savePresetButton);
    addAndMakeVisible(loadPresetButton);
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
    chooser.launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();
        if (file.exists())
        {
            if (!AudioEngine::getInstance().loadPlugin(file))
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "Failed to load plugin.");
            }
        }
    });
}

void MainComponent::onSavePreset()
{
    juce::FileChooser chooser("Save preset file...", {}, "*.preset");
    chooser.launchAsync(juce::FileBrowserComponent::saveMode, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();
        if (!file.getFullPathName().isEmpty())
        {
            if (!AudioEngine::getInstance().savePreset(file))
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "Failed to save preset.");
            }
        }
    });
}

void MainComponent::onLoadPreset()
{
    juce::FileChooser chooser("Select a preset file to load...", {}, "*.preset");
    chooser.launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();
        if (file.exists())
        {
            if (!AudioEngine::getInstance().loadPreset(file))
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "Failed to load preset.");
            }
        }
    });
}

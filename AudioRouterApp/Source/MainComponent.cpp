void MainComponent::loadPlugin()
{
    juce::FileChooser chooser("Select a plugin to load...", {}, "*.vst3;*.vst;*.component");

    chooser.launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file.exists())
        {
            auto result = audioEngine.loadPlugin(file);
            if (!result)
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "Failed to load plugin.");
            }
        }
    });
}

void MainComponent::savePreset()
{
    juce::FileChooser chooser("Save preset file...", {}, "*.preset");

    chooser.launchAsync(juce::FileBrowserComponent::saveMode, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (!file.getFullPathName().isEmpty())
        {
            auto result = audioEngine.savePreset(file);
            if (!result)
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "Failed to save preset.");
            }
        }
    });
}

void MainComponent::loadPreset()
{
    juce::FileChooser chooser("Select a preset file to load...", {}, "*.preset");

    chooser.launchAsync(juce::FileBrowserComponent::openMode, [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        if (file.exists())
        {
            auto result = audioEngine.loadPreset(file);
            if (!result)
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                                       "Error",
                                                       "Failed to load preset.");
            }
        }
    });
}

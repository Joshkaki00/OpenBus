#include "AudioEngine.h"
#include <juce_gui_basics/juce_gui_basics.h>

class AudioRouterApp : public juce::JUCEApplication
{
public:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* component)
            : juce::DocumentWindow(name, juce::Colours::lightgrey, juce::DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(component, true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    AudioRouterApp() = default;

    const juce::String getApplicationName() override { return "AudioRouterApp"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow("AudioRouterApp", new MainComponent()));
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

private:
    std::unique_ptr<MainWindow> mainWindow;
};

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        loadPluginButton.onClick = [this]() { onLoadPlugin(); };
        savePresetButton.onClick = [this]() { onSavePreset(); };
        loadPresetButton.onClick = [this]() { onLoadPreset(); };

        addAndMakeVisible(loadPluginButton);
        addAndMakeVisible(savePresetButton);
        addAndMakeVisible(loadPresetButton);

        setSize(600, 400);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(10);
        loadPluginButton.setBounds(area.removeFromTop(30));
        savePresetButton.setBounds(area.removeFromTop(30));
        loadPresetButton.setBounds(area.removeFromTop(30));
    }

private:
    juce::TextButton loadPluginButton{ "Load Plugin" };
    juce::TextButton savePresetButton{ "Save Preset" };
    juce::TextButton loadPresetButton{ "Load Preset" };

    AudioEngine audioEngine;

    void onLoadPlugin()
    {
        juce::FileChooser chooser("Select a plugin to load...", {}, "*");
        chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (!audioEngine.loadPlugin(file))
                    {
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                            "Error",
                            "Failed to load plugin.");
                    }
                }
            });
    }

    void onSavePreset()
    {
        juce::FileChooser chooser("Save Preset...", {}, "*");
        chooser.launchAsync(juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (!audioEngine.savePreset(file))
                    {
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                            "Error",
                            "Failed to save preset.");
                    }
                }
            });
    }

    void onLoadPreset()
    {
        juce::FileChooser chooser("Load Preset...", {}, "*");
        chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (!audioEngine.loadPreset(file))
                    {
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                            "Error",
                            "Failed to load preset.");
                    }
                }
            });
    }
};

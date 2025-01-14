#include <iostream>
#include <thread>
#include "AudioEngine.h"
#include "ZeroMQServer.h"
#include <juce_gui_basics/juce_gui_basics.h>

// Custom GUI application class
class AudioRouterApp : public juce::JUCEApplication
{
public:
    AudioRouterApp() = default;

    const juce::String getApplicationName() override { return "Audio Router App"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String&) override
    {
        // Create main GUI window
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        // Clean up
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String&) override {}

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name)
            : DocumentWindow(name,
                             juce::Desktop::getInstance().getDefaultLookAndFeel()
                                 .findColour(juce::ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        class MainComponent : public juce::Component
        {
        public:
            MainComponent()
            {
                setSize(400, 300);

                // Set up buttons
                loadPluginButton.setButtonText("Load Plugin");
                savePresetButton.setButtonText("Save Preset");
                loadPresetButton.setButtonText("Load Preset");

                // Add buttons to the component
                addAndMakeVisible(loadPluginButton);
                addAndMakeVisible(savePresetButton);
                addAndMakeVisible(loadPresetButton);

                // Attach button listeners
                loadPluginButton.onClick = [this]() { onLoadPlugin(); };
                savePresetButton.onClick = [this]() { onSavePreset(); };
                loadPresetButton.onClick = [this]() { onLoadPreset(); };
            }

            void resized() override
            {
                auto area = getLocalBounds();
                loadPluginButton.setBounds(area.removeFromTop(40).reduced(10));
                savePresetButton.setBounds(area.removeFromTop(40).reduced(10));
                loadPresetButton.setBounds(area.removeFromTop(40).reduced(10));
            }

        private:
            void onLoadPlugin()
            {
                juce::FileChooser chooser("Select a plugin to load...", {}, "*");
                if (chooser.browseForFileToOpen())
                {
                    juce::File file = chooser.getResult();
                    if (!audioEngine.loadPlugin(file))
                    {
                        juce::AlertWindow::showMessageBoxAsync(
                            juce::AlertWindow::WarningIcon,
                            "Error",
                            "Failed to load plugin.");
                    }
                }
            }

            void onSavePreset()
            {
                juce::FileChooser chooser("Save Preset...", {}, "*");
                if (chooser.browseForFileToSave(true))
                {
                    juce::File file = chooser.getResult();
                    if (!audioEngine.savePreset(file))
                    {
                        juce::AlertWindow::showMessageBoxAsync(
                            juce::AlertWindow::WarningIcon,
                            "Error",
                            "Failed to save preset.");
                    }
                }
            }

            void onLoadPreset()
            {
                juce::FileChooser chooser("Load Preset...", {}, "*");
                if (chooser.browseForFileToOpen())
                {
                    juce::File file = chooser.getResult();
                    if (!audioEngine.loadPreset(file))
                    {
                        juce::AlertWindow::showMessageBoxAsync(
                            juce::AlertWindow::WarningIcon,
                            "Error",
                            "Failed to load preset.");
                    }
                }
            }

            AudioEngine audioEngine;

            juce::TextButton loadPluginButton;
            juce::TextButton savePresetButton;
            juce::TextButton loadPresetButton;
        };
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

// Main entry point
START_JUCE_APPLICATION(AudioRouterApp)

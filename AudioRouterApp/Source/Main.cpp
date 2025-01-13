#include <JuceHeader.h>
#include "MainComponent.h"
#include "AudioEngine.h"

class AudioRouterApplication : public juce::JUCEApplication
{
public:
    // Return the name of the application
    const juce::String getApplicationName() override { return "Audio Router"; }

    // Return the version of the application
    const juce::String getApplicationVersion() override { return "1.0.0"; }

    // Called when the application starts
    void initialise(const juce::String&) override
    {
        audioEngine = std::make_unique<AudioEngine>();

        // Create the main application window
        mainWindow = std::make_unique<MainWindow>("Audio Router", *audioEngine);
    }

    // Called when the application is shutting down
    void shutdown() override
    {
        mainWindow = nullptr; // Clean up the main window
        audioEngine = nullptr; // Clean up the audio engine
    }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, AudioEngine& engine)
            : juce::DocumentWindow(name,
                                   juce::Colours::darkgrey,
                                   DocumentWindow::allButtons)
        {
            // Set the main component for the window
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(engine), true);

            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            // Handle application close event
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow; // The main application window
    std::unique_ptr<AudioEngine> audioEngine; // The audio engine
};

// Start the JUCE application
START_JUCE_APPLICATION(AudioRouterApplication)

#include "AudioEngine.h"
#include <JuceHeader.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "MainComponent.h"


class AudioRouterApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "AudioRouterApp"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String& commandLine) override
    {
        if (commandLine.isNotEmpty())
        {
            // Run in command-line mode
            runCommandLineMode(commandLine);
        }
        else
        {
            // Start GUI mode
            mainWindow.reset(new MainWindow("AudioRouterApp", new MainComponent(), *this));
        }
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* c, JUCEApplication& app)
            : DocumentWindow(name, juce::Colours::lightgrey, DocumentWindow::allButtons),
              owner(app)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(c, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCEApplication& owner;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    std::unique_ptr<MainWindow> mainWindow;

    void runCommandLineMode(const juce::String& commandLine)
    {
        DBG("Running in command-line mode");
        DBG("Arguments: " << commandLine);

        // Parse command-line arguments
        juce::StringArray args = juce::StringArray::fromTokens(commandLine, true);

        if (args.contains("--help"))
        {
            DBG("Usage: AudioRouterApp [options]");
            DBG("--help: Show this help message");
            DBG("--load-plugin <path>: Load a plugin at the specified path");
        }
        else if (args.contains("--load-plugin"))
        {
            int index = args.indexOf("--load-plugin");
            if (index + 1 < args.size())
            {
                juce::String pluginPath = args[index + 1];
                DBG("Loading plugin: " << pluginPath);
                // Add your plugin-loading logic here
            }
            else
            {
                DBG("Error: Missing plugin path");
            }
        }

        // Quit after execution
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
};

// Start the JUCE application
START_JUCE_APPLICATION(AudioRouterApp)

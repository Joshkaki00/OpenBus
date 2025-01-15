#include "AudioEngine.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include "MainComponent.h"
#include "Logger.h"

class AudioRouterApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "AudioRouterApp"; }
    const juce::String getApplicationVersion() override { return "1.0.0"; }

    void initialise(const juce::String&) override
    {
        // Specify the log file path as "juce_log.txt"
        juce::File logFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                                 .getChildFile("juce_log.txt");

        // Set the custom logger
        juce::Logger::setCurrentLogger(new juce::FileLogger(logFile, "Application Log Started"));

        // Log an initialization message
        juce::Logger::writeToLog("Application started.");

        mainWindow.reset(new MainWindow("AudioRouterApp", new MainComponent(), *this));
    }

    void shutdown() override
    {
        juce::Logger::writeToLog("Application shutting down.");
        juce::Logger::setCurrentLogger(nullptr); // Reset the logger to avoid leaks
        mainWindow = nullptr;
    }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* c, juce::JUCEApplication& app)
            : juce::DocumentWindow(name, juce::Colours::lightgrey, juce::DocumentWindow::allButtons),
              application(app)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(c, true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            application.systemRequestedQuit();
        }

    private:
        juce::JUCEApplication& application;
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(AudioRouterApp)

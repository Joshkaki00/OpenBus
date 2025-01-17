#include "AudioEngine.h"
#include <JuceHeader.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "MainComponent.h"


class AudioRouterApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "AudioRouterApp"; }
    const juce::String getApplicationVersion() override { return "0.0.1"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise(const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow("AudioRouterApp", new MainComponent(), *this));
        
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
};

// Start the JUCE application
START_JUCE_APPLICATION(AudioRouterApp)

#include <JuceHeader.h>
#include "AudioEngine.h"
#include "ZeroMQServer.h"
#include "MainComponent.h"
#include "Config.h"

class MainApplication : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "Audio Engine"; }
    const juce::String getApplicationVersion() override { return "1.0"; }

    void initialise(const juce::String&) override
    {
        audioEngine = std::make_unique<AudioEngine>();

        // Initialize plugin formats
        zmqServer = std::make_unique<ZeroMQServer>(*audioEngine);
        zmqThread = std::thread([this]() { zmqServer->listen(); });

        mainWindow = std::make_unique<MainWindow>("AudioRouterApp", new MainComponent(), *this);
        juce::Process::setDockIconVisible(false); // Optional: Hide dock icon
    }

    void shutdown() override
    {
        zmqThread.join();
        audioEngine.reset();
        zmqServer.reset();
        mainWindow = nullptr;
    }

private:
    std::unique_ptr<AudioEngine> audioEngine;
    std::unique_ptr<ZeroMQServer> zmqServer;
    std::thread zmqThread;

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* c, JUCEApplication& app)
            : juce::DocumentWindow(name, juce::Colours::darkgrey, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(c, true);
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(MainApplication)

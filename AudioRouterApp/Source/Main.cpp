#include <JuceHeader.h>
#include "MainComponent.h"

class PluginHostApp : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "PluginHostApp"; }
    const juce::String getApplicationVersion() override { return "0.0.1"; }

    void initialise(const juce::String&) override
    {
        mainWindow.reset(new MainWindow("Plugin Host", new MainComponent(), *this));
    }

    void shutdown() override { mainWindow = nullptr; }

private:
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(const juce::String& name, juce::Component* content, JUCEApplication& app)
            : DocumentWindow(name, juce::Colours::lightgrey, DocumentWindow::allButtons), owner(app)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(content, true);
            centreWithSize(getWidth(), getHeight());
            setVisible(true);
        }

        void closeButtonPressed() override { JUCEApplication::getInstance()->systemRequestedQuit(); }

    private:
        JUCEApplication& owner;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(PluginHostApp)

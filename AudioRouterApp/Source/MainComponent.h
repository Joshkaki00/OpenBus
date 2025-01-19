#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class MainComponent : public juce::Component
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void scanForPlugins();              // Scan for plugins in default directories
    void openSelectedPlugin();          // Open the editor for the selected plugin

    juce::TextButton scanPluginsButton{"Scan Plugins"};
    juce::TextButton openPluginButton{"Open Plugin"};
    juce::ComboBox pluginDropdown;
    juce::StringArray pluginsFound;
    std::unique_ptr<juce::DocumentWindow> editorWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

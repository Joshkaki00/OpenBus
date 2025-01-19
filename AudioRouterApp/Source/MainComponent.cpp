#include "MainComponent.h"
#include "AudioEngine.h"

MainComponent::MainComponent()
{
    setSize(800, 600);

    scanPluginsButton.onClick = [this] { scanForPlugins(); };
    openPluginButton.onClick = [this] { openSelectedPlugin(); };

    addAndMakeVisible(scanPluginsButton);
    addAndMakeVisible(pluginDropdown);
    addAndMakeVisible(openPluginButton);
}

MainComponent::~MainComponent() = default;

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

void MainComponent::resized()
{
    auto area = getLocalBounds().reduced(20);
    scanPluginsButton.setBounds(area.removeFromTop(40));
    pluginDropdown.setBounds(area.removeFromTop(40));
    openPluginButton.setBounds(area.removeFromTop(40));
}

void MainComponent::scanForPlugins()
{
    juce::StringArray directories;

#if JUCE_WINDOWS
    directories.add("C:\\Program Files\\Common Files\\VST3");
#elif JUCE_MAC
    directories.add("~/Library/Audio/Plug-Ins/VST3");
#elif JUCE_LINUX
    directories.add("~/.vst3");
#endif

    for (const auto& dir : directories)
    {
        juce::File pluginDir(dir);

        if (pluginDir.isDirectory())
        {
            DBG("Scanning directory: " << pluginDir.getFullPathName());

            for (const auto& file : juce::RangedDirectoryIterator(pluginDir, false, "*.vst3"))
            {
                pluginsFound.add(file.getFile().getFullPathName());
            }
        }
    }

    pluginDropdown.clear();
    for (int i = 0; i < pluginsFound.size(); ++i)
        pluginDropdown.addItem(pluginsFound[i], i + 1);
}

void MainComponent::openSelectedPlugin()
{
    const int index = pluginDropdown.getSelectedId() - 1;
    if (index >= 0 && index < pluginsFound.size())
    {
        if (AudioEngine::getInstance().loadPlugin(juce::File(pluginsFound[index])))
        {
            auto* editor = AudioEngine::getInstance().createEditorForPlugin(index);
            if (editor)
            {
                editorWindow.reset(new juce::DocumentWindow(
                    "Plugin Editor", juce::Colours::lightgrey, juce::DocumentWindow::allButtons));
                editorWindow->setContentNonOwned(editor, true);
                editorWindow->setResizable(true, false);
                editorWindow->centreWithSize(editor->getWidth(), editor->getHeight());
                editorWindow->setVisible(true);
            }
        }
    }
}

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
    pluginDropdown.clear();

    // Define directories to search for plugins
    juce::StringArray directories{
        "/Library/Audio/Plug-Ins/VST3", // macOS
        "C:\\Program Files\\Common Files\\VST3", // Windows
        "~/.vst3", "/usr/lib/vst3", "/usr/local/lib/vst3" // Linux
    };

    for (const auto& dir : directories)
    {
        juce::File pluginDir(dir);

        if (pluginDir.isDirectory())
        {
            // Use RangedDirectoryIterator to scan the directory
            for (const auto& file : juce::RangedDirectoryIterator(pluginDir, false, "*.vst3"))
            {
                juce::File pluginFile = file.getFile();

                // Add the plugin file to the dropdown and the list
                pluginsFound.add(pluginFile.getFullPathName());
                pluginDropdown.addItem(pluginFile.getFileName(), pluginsFound.size());
            }
        }
    }
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

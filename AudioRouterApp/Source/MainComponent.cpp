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

void scanPlugins()
{
    juce::StringArray directories;

    // Define platform-specific directories
   #if JUCE_WINDOWS
    directories.add("C:\\Program Files\\Common Files\\VST3"); // Default VST3 directory
    directories.add("C:\\Program Files (x86)\\Common Files\\VST3"); // For 32-bit plugins
    directories.add("C:\\Program Files\\Common Files\\VST"); // Default VST directory
    directories.add("C:\\Program Files (x86)\\Common Files\\VST"); // For 32-bit plugins
   #elif JUCE_MAC
    directories.add("~/Library/Audio/Plug-Ins/VST3"); // macOS user VST3 folder
    directories.add("/Library/Audio/Plug-Ins/VST3"); // macOS system VST3 folder
    directories.add("/Library/Audio/Plug_Ins/VST"); // macOS system VST folder
    directories.add("~/Library/Audio/Plug_Ins/VST"); // macOS user VST folder
   #elif JUCE_LINUX
    directories.add("~/.vst3"); // User VST3 folder
    directories.add("~/.vst"); // User VST folder
    directories.add("/usr/lib/vst3"); // System VST3 folder
    directories.add("/usr/lib/vst"); // System VST folder
    directories.add("/usr/local/lib/vst3"); // Local system VST3 folder
    directories.add("/usr/local/lib/vst"); // Local system VST folder
   #endif

    // Scan for plugins in each directory
    for (const auto& dir : directories)
    {
        juce::File pluginDir(dir);

        if (pluginDir.isDirectory())
        {
            DBG("Scanning directory: " << pluginDir.getFullPathName());

            // Use RangedDirectoryIterator to find all VST3 plugins
            for (const auto& file : juce::RangedDirectoryIterator(pluginDir, false, "*.vst3"))
            {
                juce::File pluginFile = file.getFile();

                // Log or process the plugin file
                DBG("Found plugin: " << pluginFile.getFullPathName());
            }
        }
        else
        {
            DBG("Invalid or inaccessible directory: " << dir);
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

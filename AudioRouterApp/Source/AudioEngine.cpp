#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    // Initialize audio graph or other processing structures
}

AudioEngine::~AudioEngine()
{
    // Cleanup resources
}

bool AudioEngine::loadPlugin(const juce::File& file)
{
    juce::PluginDescription pluginDesc;
    auto formatManager = juce::AudioPluginFormatManager();
    formatManager.addDefaultFormats();

    auto plugin = formatManager.createPluginInstance(pluginDesc, 44100.0, 512, {});
    if (plugin == nullptr)
        return false;

    currentNode = graph.addNode(std::move(plugin));
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    juce::XmlElement preset("Preset");
    if (currentNode != nullptr)
    {
        auto plugin = currentNode->getProcessor();
        juce::XmlElement* state = plugin->getStateInformation();
        if (state != nullptr)
        {
            preset.addChildElement(state);
            return preset.writeToFile(file, {});
        }
    }
    return false;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    auto xml = juce::XmlDocument::parse(file);
    if (xml == nullptr)
        return false;

    if (currentNode != nullptr)
    {
        auto plugin = currentNode->getProcessor();
        plugin->setStateInformation(xml.get());
        return true;
    }
    return false;
}

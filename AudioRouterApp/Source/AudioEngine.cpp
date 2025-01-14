#include "AudioEngine.h"
#include <iostream> // For logging

// Constructor
AudioEngine::AudioEngine()
{
    initializeFormats();
    setupGraph();
}

// Setup Audio Graph
void AudioEngine::setupGraph()
{
    for (int i = 0; i < maxInputs; ++i)
    {
        inputNode[i] = addNode(createIOProcessor(true)).get();
        outputNode[i] = addNode(createIOProcessor(false)).get();
    }
}

// Handle JSON Commands
void AudioEngine::handleCommand(const std::string& msg)
{
    try
    {
        auto command = json::parse(msg);

        if (!command.contains("action"))
        {
            std::cerr << "Command missing 'action' key." << std::endl;
            return;
        }

        const auto action = command["action"].get<std::string>();

        if (action == "add_plugin")
        {
            if (command.contains("path") && command.contains("input"))
            {
                std::string pluginName = command["path"].get<std::string>();
                int input = command["input"].get<int>();

                addPlugin(juce::String(pluginName), input);
            }
            else
            {
                std::cerr << "Missing 'path' or 'input' for add_plugin command." << std::endl;
            }
        }
        else
        {
            std::cerr << "Unknown action specified: " << action << std::endl;
        }
    }
    catch (const json::exception& e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

// Initialize plugin formats
void AudioEngine::initializeFormats()
{
    if (formatManager.getNumFormats() == 0)
    {
        formatManager.addDefaultFormats();
    }
}

// Load Plugin
std::unique_ptr<juce::AudioProcessor> AudioEngine::loadPlugin(const juce::String& path)
{
    initializeFormats();

    juce::String errorMessage;
    juce::File pluginFile(path);

    if (!pluginFile.existsAsFile())
    {
        std::cerr << "Invalid plugin path: " << path << std::endl;
        return nullptr;
    }

    juce::PluginDescription desc;
    juce::AudioPluginFormat* format = formatManager.getFormat(0);
    juce::OwnedArray<juce::PluginDescription> results;

    format->findAllTypesForFile(results, pluginFile.getFullPathName());

    if (results.isEmpty())
    {
        std::cerr << "No matching plugins found for: " << path << std::endl;
        return nullptr;
    }

    auto plugin = formatManager.createPluginInstance(*results[0], 44100.0, 512, errorMessage);

    if (!plugin)
    {
        std::cerr << "Failed to load plugin: " << errorMessage.toStdString() << std::endl;
        return nullptr;
    }

    std::cerr << "Plugin loaded successfully: " << results[0]->name.toStdString() << std::endl;
    return plugin;
}

// Add Plugin
void AudioEngine::addPlugin(const juce::String& path, int inputIndex)
{
    if (inputIndex < 0 || inputIndex >= maxInputs)
    {
        std::cerr << "Invalid input index specified." << std::endl;
        return;
    }

    auto plugin = loadPlugin(path);

    if (plugin)
    {
        auto node = addNode(std::move(plugin));

        for (int ch = 0; ch < maxInputs; ++ch)
        {
            if (inputNode[ch] && outputNode[ch])
            {
                addConnection({ { inputNode[ch]->nodeID, 0 }, { node->nodeID, 0 } });
                addConnection({ { node->nodeID, 0 }, { outputNode[ch]->nodeID, 0 } });
            }
        }

        std::cerr << "Plugin added successfully to input: " << inputIndex << std::endl;
    }
    else
    {
        std::cerr << "Failed to add plugin at path: " << path << std::endl;
    }
}

// Create IO Processor
std::unique_ptr<juce::AudioProcessor> AudioEngine::createIOProcessor(bool isInput)
{
    return std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
        isInput ? juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode
                : juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode
    );
}

// Setup Virtual Output
void AudioEngine::setupVirtualOutput(const juce::String& outputName)
{
    juce::AudioDeviceManager deviceManager;

    auto* currentDevice = deviceManager.getCurrentAudioDevice();
    if (currentDevice != nullptr)
    {
        juce::StringArray outputs = currentDevice->getOutputChannelNames();
        if (outputs.contains(outputName))
        {
            std::cerr << "Output set to: " << outputName << std::endl;
        }
        else
        {
            std::cerr << "Output not found!" << std::endl;
        }
    }
    else
    {
        std::cerr << "No audio device available!" << std::endl;
    }
}

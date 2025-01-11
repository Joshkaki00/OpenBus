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
    try
    {
        for (int i = 0; i < maxInputs; ++i)
        {
            inputNode[i] = addNode(createIOProcessor(true)).get();
            outputNode[i] = addNode(createIOProcessor(false)).get();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Graph setup failed: " << e.what() << std::endl;
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

                // Map plugin names to absolute paths
                std::map<std::string, std::string> pluginPaths = {
                    {"Limiter", "/Users/Plugins/Limiter.vst3"},
                    {"EQ", "/Users/Plugins/EQ.vst3"}
                };

                if (pluginPaths.find(pluginName) != pluginPaths.end())
                {
                    juce::File pluginFile(pluginPaths[pluginName]);

                    if (pluginFile.existsAsFile())
                    {
                        addPlugin(pluginFile.getFullPathName(), input);
                    }
                    else
                    {
                        std::cerr << "Invalid plugin path: " << pluginFile.getFullPathName().toStdString() << std::endl;
                    }
                }
                else
                {
                    std::cerr << "Plugin not found: " << pluginName << std::endl;
                }
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


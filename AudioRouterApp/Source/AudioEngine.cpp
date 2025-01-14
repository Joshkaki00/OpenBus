#include "AudioEngine.h"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Constructor
AudioEngine::AudioEngine() : currentInputId(0), currentOutputId(0)
{
    // Initialization code
}

// Destructor
AudioEngine::~AudioEngine()
{
    // Cleanup code
}

// Set Input
void AudioEngine::setInput(int inputId)
{
    currentInputId = inputId;
    std::cout << "Input set to: " << currentInputId << std::endl;

    // Add your input routing logic here
}

// Set Output
void AudioEngine::setOutput(int outputId)
{
    currentOutputId = outputId;
    std::cout << "Output set to: " << currentOutputId << std::endl;

    // Add your output routing logic here
}

// Handle JSON Command
void AudioEngine::handleCommand(const std::string& command)
{
    try
    {
        // Parse the JSON command
        auto parsedCommand = json::parse(command);

        if (!parsedCommand.contains("action"))
        {
            std::cerr << "Command missing 'action' key." << std::endl;
            return;
        }

        const auto action = parsedCommand["action"].get<std::string>();

        if (action == "set_input" && parsedCommand.contains("input"))
        {
            int inputId = parsedCommand["input"].get<int>();
            setInput(inputId);
        }
        else if (action == "set_output" && parsedCommand.contains("output"))
        {
            int outputId = parsedCommand["output"].get<int>();
            setOutput(outputId);
        }
        else
        {
            std::cerr << "Unknown action or missing parameters in command." << std::endl;
        }
    }
    catch (const json::exception& e)
    {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

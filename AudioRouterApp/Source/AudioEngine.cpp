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


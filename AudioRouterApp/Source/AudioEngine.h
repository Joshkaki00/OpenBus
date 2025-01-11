#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <JuceHeader.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <array>

using json = nlohmann::json;

// AudioEngine Class
class AudioEngine : public juce::AudioProcessorGraph
{
public:
    // Constructor and Destructor
    AudioEngine();
    ~AudioEngine() override = default;

    // Public Methods
    void setupGraph();                                      // Setup the audio graph
    void handleCommand(const std::string& msg);             // Handle JSON commands
    void addPlugin(const juce::String& path, int inputIndex); // Add plugin dynamically
    void setPluginParam(const juce::String& pluginId,       // Set plugin parameter
                        const juce::String& paramId, float value);
    void initializeFormats();                               // Initialize plugin formats
    void setupVirtualOutput(const juce::String& outputName);// Virtual output configuration


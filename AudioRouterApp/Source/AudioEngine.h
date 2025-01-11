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

private:
    // Constants
    static constexpr int maxInputs = 6; // Max number of input/output channels

    // Nodes for inputs and outputs
    std::array<juce::AudioProcessorGraph::Node*, maxInputs> inputNode;
    std::array<juce::AudioProcessorGraph::Node*, maxInputs> outputNode;

    // Plugin Management
    juce::AudioPluginFormatManager formatManager; // Plugin format manager
    juce::KnownPluginList knownPlugins;          // Tracks available plugins
    juce::StringArray deadMansPedal;             // Crash recovery list

    // Private Methods
    std::unique_ptr<juce::AudioProcessor> createIOProcessor(bool isInput); // Create IO processors
    std::unique_ptr<juce::AudioProcessor> loadPlugin(const juce::String& path); // Load plugins safely

    // Copy and Move prevention
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;
};

#endif // AUDIOENGINE_H

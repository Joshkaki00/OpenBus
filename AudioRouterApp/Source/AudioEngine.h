#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <JuceHeader.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <array>

using json = nlohmann::json;

class AudioEngine : public juce::AudioProcessorGraph
{
public:
    AudioEngine();
    ~AudioEngine() override = default;

    void setupGraph();
    void handleCommand(const std::string& msg);
    void addPlugin(const juce::String& path, int inputIndex);
    void setPluginParam(const juce::String& pluginId, const juce::String& paramId, float value);
    void initializeFormats();
    void setupVirtualOutput(const juce::String& outputName);

private:
    static constexpr int maxInputs = 6;
    std::array<juce::AudioProcessorGraph::Node*, maxInputs> inputNode;
    std::array<juce::AudioProcessorGraph::Node*, maxInputs> outputNode;

    juce::AudioPluginFormatManager formatManager;
    juce::KnownPluginList knownPlugins;
    juce::StringArray deadMansPedal;

    std::unique_ptr<juce::AudioProcessor> createIOProcessor(bool isInput);
    std::unique_ptr<juce::AudioProcessor> loadPlugin(const juce::String& path);

    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;
};

#endif // AUDIOENGINE_H

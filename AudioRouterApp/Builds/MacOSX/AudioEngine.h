#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <JuceHeader.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class AudioEngine : public juce::AudioProcessorGraph
{
public:
    AudioEngine();
    void setupGraph();
    void handleCommand(const std::string& msg);
    void addPlugin(const std::string& path, int inputIndex);
    void setPluginParam(int pluginId, int paramId, float value);

private:
    const int maxInputs = 6;
    Node::Ptr inputNode[6];
    Node::Ptr outputNode[6];

    Node::Ptr createIOProcessor(bool isInput, int index);
    juce::AudioProcessor* loadPlugin(const std::string& path);
};

#endif // AUDIOENGINE_H

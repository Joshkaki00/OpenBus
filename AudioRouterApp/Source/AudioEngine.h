#pragma once

#include <JuceHeader.h>

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

private:
    juce::AudioProcessorGraph graph;
    juce::AudioProcessorGraph::Node::Ptr currentNode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

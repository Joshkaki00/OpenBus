#pragma once

#include <JuceHeader.h>
#include <vector>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    bool loadPlugin(const juce::File& file);
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);

    // New functions
    nlohmann::json getDeviceList();
    nlohmann::json setInputDevice(const std::string& deviceName);
    nlohmann::json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioProcessorGraph graph;
    juce::AudioProcessorGraph::Node::Ptr currentNode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

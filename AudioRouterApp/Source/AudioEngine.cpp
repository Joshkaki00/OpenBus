#include "AudioEngine.h"
#include <iostream>

AudioEngine::AudioEngine() {
    initializeFormats();
    setupGraph();
}

void AudioEngine::setupGraph() {
    try {
        for (int i = 0; i < maxInputs; ++i) {
            inputNode[i] = addNode(createIOProcessor(true)).get();
            outputNode[i] = addNode(createIOProcessor(false)).get();
        }
    } catch (const std::exception& e) {
        std::cerr << "Graph setup failed: " << e.what() << std::endl;
    }
}

void AudioEngine::handleCommand(const std::string& msg) {
    try {
        auto command = json::parse(msg);
        if (!command.contains("action")) throw std::runtime_error("Missing 'action' key");

        const auto action = command["action"].get<std::string>();
        if (action == "add_plugin" && command.contains("path") && command.contains("input")) {
            addPlugin(juce::String(command["path"].get<std::string>()), command["input"].get<int>());
        } else {
            std::cerr << "Unknown or incomplete command: " << action << std::endl;
        }
    } catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
}

void AudioEngine::initializeFormats() {
    if (formatManager.getNumFormats() == 0) formatManager.addDefaultFormats();
}

std::unique_ptr<juce::AudioProcessor> AudioEngine::loadPlugin(const juce::String& path) {
    initializeFormats();
    juce::File pluginFile(path);
    if (!pluginFile.existsAsFile()) throw std::runtime_error("Plugin file not found");

    juce::OwnedArray<juce::PluginDescription> results;
    formatManager.getFormat(0)->findAllTypesForFile(results, pluginFile.getFullPathName());
    if (results.isEmpty()) throw std::runtime_error("No matching plugins found");

    juce::String errorMessage;
    return formatManager.createPluginInstance(*results[0], 44100.0, 512, errorMessage);
}

void AudioEngine::addPlugin(const juce::String& path, int inputIndex) {
    if (inputIndex < 0 || inputIndex >= maxInputs) throw std::out_of_range("Invalid input index");

    auto plugin = loadPlugin(path);
    auto node = addNode(std::move(plugin));
    addConnection({ { inputNode[inputIndex]->nodeID, 0 }, { node->nodeID, 0 } });
    addConnection({ { node->nodeID, 0 }, { outputNode[inputIndex]->nodeID, 0 } });

    std::cerr << "Plugin added to input " << inputIndex << std::endl;
}

std::unique_ptr<juce::AudioProcessor> AudioEngine::createIOProcessor(bool isInput) {
    return std::make_unique<juce::AudioProcessorGraph::AudioGraphIOProcessor>(
        isInput ? juce::AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode
                : juce::AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode);
}

void AudioEngine::setupVirtualOutput(const juce::String& outputName) {
    juce::AudioDeviceManager deviceManager;
    auto* currentDevice = deviceManager.getCurrentAudioDevice();
    if (currentDevice && currentDevice->getOutputChannelNames().contains(outputName)) {
        std::cerr << "Virtual output set to: " << outputName << std::endl;
    } else {
        std::cerr << "Output not found!" << std::endl;
    }
}

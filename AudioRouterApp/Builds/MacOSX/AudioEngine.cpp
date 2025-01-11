#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    setupGraph();
}

void AudioEngine::setupGraph()
{
    for (int i = 0; i < maxInputs; ++i)
    {
        inputNode[i] = addNode(createIOProcessor(true, i));
        outputNode[i] = addNode(createIOProcessor(false, i));
    }
}

void AudioEngine::handleCommand(const std::string& msg)
{
    auto command = json::parse(msg);

    if (command["action"] == "add_plugin")
    {
        addPlugin(command["path"], command["input"]);
    }
    else if (command["action"] == "set_param")
    {
        setPluginParam(command["plugin_id"], command["param_id"], command["value"]);
    }
}

void AudioEngine::addPlugin(const std::string& path, int inputIndex)
{
    auto plugin = loadPlugin(path);
    if (plugin)
    {
        auto nodeId = addNode(plugin)->nodeID;
        connect(inputNode[inputIndex]->nodeID, nodeId);
        connect(nodeId, outputNode[inputIndex]->nodeID);
    }
}

void AudioEngine::setPluginParam(int pluginId, int paramId, float value)
{
    auto node = getNodeForId(juce::NodeID(pluginId));
    if (node)
        node->getProcessor()->setParameter(paramId, value);
}

juce::AudioProcessor* AudioEngine::loadPlugin(const std::string& path)
{
    // Placeholder for LV2 loader
    return nullptr;
}

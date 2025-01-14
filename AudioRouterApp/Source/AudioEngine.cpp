#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    deviceManager.initialise(2, 2, nullptr, true); // Initialize with 2 input and 2 output channels
}

AudioEngine::~AudioEngine() {}

bool AudioEngine::loadPlugin(const juce::File& file)
{
    juce::PluginDescription pluginDesc;
    auto formatManager = juce::AudioPluginFormatManager();
    formatManager.addDefaultFormats();

    auto plugin = formatManager.createPluginInstance(pluginDesc, 44100.0, 512, {});
    if (plugin == nullptr)
        return false;

    currentNode = graph.addNode(std::move(plugin));
    return true;
}

bool AudioEngine::savePreset(const juce::File& file)
{
    juce::XmlElement preset("Preset");
    if (currentNode != nullptr)
    {
        auto plugin = currentNode->getProcessor();
        juce::XmlElement* state = plugin->getStateInformation();
        if (state != nullptr)
        {
            preset.addChildElement(state);
            return preset.writeToFile(file, {});
        }
    }
    return false;
}

bool AudioEngine::loadPreset(const juce::File& file)
{
    auto xml = juce::XmlDocument::parse(file);
    if (xml == nullptr)
        return false;

    if (currentNode != nullptr)
    {
        auto plugin = currentNode->getProcessor();
        plugin->setStateInformation(xml.get());
        return true;
    }
    return false;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json deviceList;

    auto availableInputs = deviceManager.getAvailableDeviceNames(true); // true = input devices
    auto availableOutputs = deviceManager.getAvailableDeviceNames(false); // false = output devices

    for (auto& input : availableInputs)
        deviceList["inputs"].push_back(input.toStdString());

    for (auto& output : availableOutputs)
        deviceList["outputs"].push_back(output.toStdString());

    return deviceList;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    auto* currentSetup = deviceManager.getCurrentAudioDeviceSetup();

    currentSetup->inputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(*currentSetup, true); // true = apply immediately

    if (result.wasOk())
        return {{"status", "success"}, {"message", "Input device set successfully"}};
    else
        return {{"status", "error"}, {"message", result.getErrorMessage().toStdString()}};
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    auto* currentSetup = deviceManager.getCurrentAudioDeviceSetup();

    currentSetup->outputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(*currentSetup, true); // true = apply immediately

    if (result.wasOk())
        return {{"status", "success"}, {"message", "Output device set successfully"}};
    else
        return {{"status", "error"}, {"message", result.getErrorMessage().toStdString()}};
}

#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    pluginFormatManager.addDefaultFormats();
    deviceManager.initialiseWithDefaultDevices(2, 2);
}

AudioEngine::~AudioEngine() = default;

bool AudioEngine::loadPlugin(const juce::File& file)
{
    if (!file.existsAsFile()) return false;

    juce::PluginDescription pluginDesc;
    juce::String error;

    // Manually populate PluginDescription
    pluginDesc.fileOrIdentifier = file.getFullPathName();
    pluginDesc.pluginFormatName = "VST3"; // Or set dynamically based on file type

    // Create the plugin instance
    auto instance = pluginFormatManager.createPluginInstance(pluginDesc, 44100.0, 512, error);

    if (instance)
    {
        loadedPlugins.add(std::move(instance));
        return true;
    }

    DBG("Failed to load plugin: " << error);
    return false;
}

juce::AudioProcessorEditor* AudioEngine::createEditorForPlugin(int index)
{
    if (auto* plugin = loadedPlugins[index])
    {
        return plugin->hasEditor() ? plugin->createEditorIfNeeded() : nullptr;
    }
    return nullptr;
}

nlohmann::json AudioEngine::getDeviceList()
{
    nlohmann::json response;
    juce::StringArray inputs = deviceManager.getCurrentAudioDevice()->getInputChannelNames();
    juce::StringArray outputs = deviceManager.getCurrentAudioDevice()->getOutputChannelNames();

    response["inputs"] = inputs.toStdString();
    response["outputs"] = outputs.toStdString();
    return response;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.inputDeviceName = deviceName;

    auto error = deviceManager.setAudioDeviceSetup(setup, true);
    response["status"] = error.isEmpty() ? "success" : "error";
    response["message"] = error.toStdString();
    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    nlohmann::json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = deviceName;

    auto error = deviceManager.setAudioDeviceSetup(setup, true);
    response["status"] = error.isEmpty() ? "success" : "error";
    response["message"] = error.toStdString();
    return response;
}

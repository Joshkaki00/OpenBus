#include <nlohmann/json.hpp>
#include <juce_audio_devices/juce_audio_devices.h>

using json = nlohmann::json;

class AudioEngine
{
public:
    json getDeviceList();
    json setInputDevice(const std::string& deviceName);
    json setOutputDevice(const std::string& deviceName);

private:
    juce::AudioDeviceManager deviceManager;
};

// Method to fetch available audio devices
json AudioEngine::getDeviceList()
{
    json devices;
    json inputs, outputs;

    auto deviceTypes = deviceManager.getAvailableDeviceTypes();

    for (const auto* type : deviceTypes)
    {
        type->scanForDevices();
        auto inputNames = type->getDeviceNames(true);  // Get input devices
        auto outputNames = type->getDeviceNames(false); // Get output devices

        for (const auto& input : inputNames)
            inputs.push_back(input.toStdString());

        for (const auto& output : outputNames)
            outputs.push_back(output.toStdString());
    }

    devices["inputs"] = inputs;
    devices["outputs"] = outputs;

    return devices;
}

// Method to set input device
json AudioEngine::setInputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.inputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.wasOk()) // Check if the setup succeeded
    {
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString(); // Error details
    }

    return response;
}

json AudioEngine::setOutputDevice(const std::string& deviceName)
{
    json response;
    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);

    setup.outputDeviceName = deviceName;
    auto result = deviceManager.setAudioDeviceSetup(setup, true);

    if (result.wasOk()) // Check if the setup succeeded
    {
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    }
    else
    {
        response["status"] = "error";
        response["message"] = result.getErrorMessage().toStdString(); // Error details
    }

    return response;
}

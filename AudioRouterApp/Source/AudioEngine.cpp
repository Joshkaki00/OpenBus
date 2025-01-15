#include "AudioEngine.h"
#include <nlohmann/json.hpp>
#include <juce_audio_devices/juce_audio_devices.h>

using json = nlohmann::json;

AudioEngine::AudioEngine() {
    deviceManager.initialise(2, 2, nullptr, true);
}

nlohmann::json AudioEngine::getDeviceList() const {
    nlohmann::json response;

    // Get the list of available device types
    juce::OwnedArray<juce::AudioIODeviceType> types;
    deviceManager.createAudioDeviceTypes(types);

    for (auto* type : types) {
        type->scanForDevices();
        auto inputDevices = type->getDeviceNames(true);  // true for input devices
        auto outputDevices = type->getDeviceNames(false); // false for output devices

        response["inputs"] = inputDevices;
        response["outputs"] = outputDevices;
    }

    return response;
}

nlohmann::json AudioEngine::setInputDevice(const std::string& deviceName) {
    nlohmann::json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.inputDeviceName = deviceName;

    auto error = deviceManager.setAudioDeviceSetup(setup, true);
    if (error.isEmpty()) { // No error indicates success
        response["status"] = "success";
        response["message"] = "Input device set successfully";
    } else {
        response["status"] = "error";
        response["message"] = "Failed to set input device: " + error.toStdString();
    }

    return response;
}

nlohmann::json AudioEngine::setOutputDevice(const std::string& deviceName) {
    nlohmann::json response;

    juce::AudioDeviceManager::AudioDeviceSetup setup;
    deviceManager.getAudioDeviceSetup(setup);
    setup.outputDeviceName = deviceName;

    auto error = deviceManager.setAudioDeviceSetup(setup, true);
    if (error.isEmpty()) { // No error indicates success
        response["status"] = "success";
        response["message"] = "Output device set successfully";
    } else {
        response["status"] = "error";
        response["message"] = "Failed to set output device: " + error.toStdString();
    }

    return response;
}

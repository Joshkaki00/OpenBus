#include "AudioEngine.h"

AudioEngine::AudioEngine() {
    deviceManager.initialiseWithDefaultDevices(2, 2); // Initialize default input/output devices
}

json AudioEngine::getDeviceList() {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        auto inputDevices = currentDevice->getInputChannelNames();
        auto outputDevices = currentDevice->getOutputChannelNames();

        return {
            {"status", "success"},
            {"inputs", inputDevices.toStdVector()},
            {"outputs", outputDevices.toStdVector()}
        };
    }

    return {{"status", "error"}, {"message", "No audio device found"}};
}

json AudioEngine::setInputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        if (currentDevice->getInputChannelNames().contains(deviceName)) {
            DBG("Input device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Input device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Input device not found"}};
}

json AudioEngine::setOutputDevice(const std::string& deviceName) {
    if (auto* currentDevice = deviceManager.getCurrentAudioDevice()) {
        if (currentDevice->getOutputChannelNames().contains(deviceName)) {
            DBG("Output device set to: " << deviceName);
            return {{"status", "success"}, {"message", "Output device set successfully"}};
        }
    }
    return {{"status", "error"}, {"message", "Output device not found"}};
}

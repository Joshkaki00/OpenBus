#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <nlohmann/json.hpp> // For JSON handling

/**
 * AudioEngine handles audio-related functionalities, including managing devices.
 */
class AudioEngine
{
public:
    AudioEngine() = default;

    /** Retrieves the list of available input and output devices.
        @return A JSON object containing device lists.
    */
    nlohmann::json getDeviceList();

    /** Sets the input device by name.
        @param deviceName The name of the input device.
        @return A JSON object indicating success or failure.
    */
    nlohmann::json setInputDevice(const std::string& deviceName);

    /** Sets the output device by name.
        @param deviceName The name of the output device.
        @return A JSON object indicating success or failure.
    */
    nlohmann::json setOutputDevice(const std::string& deviceName);
};

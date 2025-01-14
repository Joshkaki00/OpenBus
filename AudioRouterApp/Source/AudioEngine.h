#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <JuceHeader.h>
#include <string>
#include <nlohmann/json.hpp>

class AudioEngine
{
public:
    // Constructor and Destructor
    AudioEngine();
    ~AudioEngine();

    // Methods for input/output
    void setInput(int inputId);
    void setOutput(int outputId);

    // Method to handle JSON commands
    void handleCommand(const std::string& command);

private:
    // Private members and methods
    int currentInputId;
    int currentOutputId;
};

#endif // AUDIOENGINE_H

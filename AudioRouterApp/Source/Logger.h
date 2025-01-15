#pragma once

#include <juce_core/juce_core.h>

class FileLogger : public juce::Logger
{
public:
    FileLogger(const juce::File& logFile)
        : logFileStream(logFile, juce::File::writeOnly)
    {
        jassert(logFileStream.openedOk());
    }

    ~FileLogger() override = default;

    void logMessage(const juce::String& message) override
    {
        logFileStream.writeText(message + "\n", false, false);
    }

private:
    juce::FileOutputStream logFileStream;

    JUCE_LEAK_DETECTOR(FileLogger) // Add this macro to detect leaks
};
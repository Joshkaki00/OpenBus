#pragma once

#include <juce_core/juce_core.h>

class FileLogger : public juce::Logger
{
public:
    FileLogger(const juce::File& logFile)
        : logFileStream(logFile.createOutputStream()) // Create the FileOutputStream
    {
        jassert(logFileStream != nullptr && logFileStream->openedOk()); // Ensure the stream is valid
    }

    ~FileLogger() override = default;

    void logMessage(const juce::String& message) override
    {
        if (logFileStream != nullptr) // Ensure the stream is valid
        {
            logFileStream->writeText(message + "\n", false, false, "\n"); // Corrected call
        }
    }

private:
    std::unique_ptr<juce::FileOutputStream> logFileStream; // Use a unique_ptr for FileOutputStream

    JUCE_LEAK_DETECTOR(FileLogger) // Add leak detector
};
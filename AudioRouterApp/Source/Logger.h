#pragma once

#include <juce_core/juce_core.h>

class FileLogger : public juce::Logger
{
public:
    // Constructor that takes a file path
    FileLogger(const juce::File& logFile);
    
    // Destructor
    ~FileLogger() override;

    // Overridden logMessage method from juce::Logger
    void logMessage(const juce::String& message) override;

private:
    std::unique_ptr<juce::FileOutputStream> logStream; // File output stream for the log file

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileLogger)
};
#include "Logger.h"

FileLogger::FileLogger(const juce::File& logFile)
{
    if (logFile.existsAsFile() || logFile.create())
        logStream = logFile.createOutputStream();
}

FileLogger::~FileLogger()
{
    logStream.reset(); // Close the file stream
}

void FileLogger::logMessage(const juce::String& message)
{
    if (logStream)
    {
        // Add timestamp to the log message
        auto timestamp = juce::Time::getCurrentTime().toString(true, true);
        logStream->writeText("[" + timestamp + "] " + message + "\n", false, false, nullptr);
        logStream->flush(); // Ensure the log message is immediately written to the file
    }
}
#include "../include/Logger.h"
#include <memory>

std::unique_ptr<std::ofstream> Logger::logFileStream;
std::unique_ptr<std::ofstream> Logger::drawFileStream;

int Logger::level = TRACE;
std::ostream* Logger::logOut(&std::cout);
std::ostream* Logger::drawOut(&std::cout);

Logger::~Logger()
{
    logFileStream->close();
    logFileStream.reset();
}

// Class functions
void Logger::print(const char* msg)
{
    *logOut << msg << std::endl;
}

void Logger::setLevel(int lvl)
{
    level = lvl;
}

void Logger::setOutput(std::ostream*& output, std::unique_ptr<std::ofstream>& ofstream, const char* filename)
{
    if(strcmp(filename, "stdout") == 0)
    {
        ofstream.reset();
        output = &std::cout;
    }
    else
    {
        ofstream = std::make_unique<std::ofstream>(filename, std::ios::app);
        if (!ofstream->is_open()) {
            ofstream.reset();
            output = &std::cout;
        }
        else
        {
            output = ofstream.get();
        }
    }
}

void Logger::setLogOutput(const char* filename)
{
    setOutput(logOut, logFileStream, filename);
    print("\n\nLOGGING STARTED\n");
}

void Logger::setDrawOutput(const char* filename)
{
    setOutput(drawOut, drawFileStream, filename);
    Draw("\n\nDRAWING STARTED\n");
}

const char* Logger::level_to_string(int level)
{
    switch(level)
    {
        case DEBUG:
            return "DEBUG";
        case TRACE:
            return "TRACE";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        default:
            return "";
    }
}

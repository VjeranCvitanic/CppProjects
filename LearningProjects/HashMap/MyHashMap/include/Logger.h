#ifndef _Logger_
#define _Logger_

#include<iostream>
#include <ostream>
#include <fstream>
#include <filesystem>

#define DEBUG 1
#define TRACE 2
#define INFO 3
#define WARNING 4
#define ERROR 5

class Logger
{
public:
    Logger(const Logger&) = delete;
    ~Logger();

    template<typename... T>
    static void Log(const char* module, const char* func, int level_, const T&... msg)
    {
        if(level_ >= level)
        {
            *logOut << level_to_string(level_) << "  ::   " << module << ":: " << func << ": ";
            (*logOut << ... << msg) << std::endl;
        }
    }
    template<typename... T>
    static void Draw(const T&... msg)
    {
        (*drawOut << ... << msg);
    }

    static void setLevel(int lvl);
    static void setLogOutput(const char* path);
    static void setDrawOutput(const char* path);
private:
    static int level;
    static std::ostream* logOut;
    static std::unique_ptr<std::ofstream> logFileStream;
    static std::ostream* drawOut;
    static std::unique_ptr<std::ofstream> drawFileStream;

    Logger(){}

    static void setOutput(std::ostream*& output, std::unique_ptr<std::ofstream>& stream, const char* path);
    static const char* level_to_string(int level);
    static void print(const char* msg);
};

#endif
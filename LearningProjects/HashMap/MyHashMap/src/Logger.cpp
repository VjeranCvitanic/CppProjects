#include "../include/Logger.h"

std::unique_ptr<std::ofstream> Logger::logFileStream;
std::unique_ptr<std::ofstream> Logger::drawFileStream;

bool Logger::is_on = false;
int Logger::level = INFO;
std::ostream* Logger::logOut(&std::cout);
std::ostream* Logger::drawOut(&std::cout);

bool Logger::is_started = false;
std::chrono::time_point<std::chrono::high_resolution_clock> Logger::start_time = {};
std::chrono::time_point<std::chrono::high_resolution_clock> Logger::end_time = {};

Logger::~Logger()
{
    Logger::end();
    print("\nLOGGING ENDED\n");
    flush();
    if(logFileStream)
    {
        logFileStream->close();
        logFileStream.reset();
    }
    if(drawFileStream)
    {
        drawFileStream->close();
        drawFileStream.reset();
    }
}

// Class functions
void Logger::logger_setup(const char* logs_dir, const char* draw_dir, int level, bool is_on)
{
    Logger::GetInstance().start();
    Logger::GetInstance().setLogOutput(logs_dir);
    Logger::GetInstance().setDrawOutput(draw_dir);
    Logger::GetInstance().setLevel(level);
    Logger::GetInstance().setOnOffState(is_on);
}

void Logger::setOnOffState(bool is_on_)
{
    is_on = is_on_;
}

void Logger::start()
{
    Logger::is_started = true;
    start_time = std::chrono::high_resolution_clock::now();
}

void Logger::end()
{
    if(is_started)
    {
        end_time = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds dur = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        print("\nExecution time: ", (float)(dur.count() / 1000.0), " miliseconds");
    }
}

template <typename... T>
void Logger::print(const T&... msg)
{
    (*logOut << ... << msg) << std::endl;
}

void Logger::setLevel(int lvl)
{
    level = lvl;
}

void Logger::setOutput(std::ostream*& output, std::unique_ptr<std::ofstream>& ofstream, const std::string& filepath)
{
    if(filepath == "stdout")
    {
        ofstream.reset();
        output = &std::cout;
    }
    else
    {
        ofstream = std::make_unique<std::ofstream>(filepath, std::ios::trunc);
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

void Logger::setLogOutput(const char* file_dir)
{
    int nextLog = 1; //getNextLogFile(file_dir)
    std::string full_filepath = std::string(file_dir) + std::string("log") + std::to_string(nextLog) + ".log";
    setOutput(logOut, logFileStream, full_filepath);
    print("\n\nLOGGING STARTED\n");
}

void Logger::setDrawOutput(const char* file_dir)
{
    int nextLog = 1; //getNextLogFile(file_dir)
    std::string full_filepath = std::string(file_dir) + std::string("drw") + std::to_string(nextLog) + ".txt";
    setOutput(drawOut, drawFileStream, full_filepath);
    Draw("\n\nDRAWING STARTED\n");
}

// TODO - timestamp
int Logger::getNextLogFile(const char* logDir)
{
    if (!std::filesystem::exists(logDir))
    {
        std::filesystem::create_directories(logDir);
    }

    std::vector<int> logNumbers;

    for (const auto& entry : std::filesystem::directory_iterator(logDir)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if ((filename.find("log") == 0 && filename.find(".log") != std::string::npos) || (filename.find("drw") == 0 && filename.find(".txt") != std::string::npos)) {
                try {
                    int num = std::stoi(filename.substr(3, filename.size() - 7)); // logX.log -> X
                    logNumbers.push_back(num);
                } catch (...) {
                    // ignore non-conforming files
                }
            }
        }
    }

    int nextLog = 0;
    if (!logNumbers.empty()) {
        nextLog = (*std::max_element(logNumbers.begin(), logNumbers.end())) % (MAX_NUM_LOGS_IN_ROTATION) + 1;
    }

    return nextLog;
}

const char* Logger::level_to_string(int level)
{
    switch(level)
    {
        case DEBUG:
            return "DEBUG";
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

void Logger::flush() {
    if (logOut)
        logOut->flush();
    if (drawOut)
        drawOut->flush();
}
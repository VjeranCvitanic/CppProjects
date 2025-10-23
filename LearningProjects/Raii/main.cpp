#include <cstdint>
#include <iostream>
#include <fcntl.h>
#include <memory>

#define _CRT_SECURE_NO_WARNINGS

// koliko puta ce se pozvat konstruktor??
// zasto Create mora biti static -> jer to nije funckija koju zelimo da se veze za neki object

class Logger {
public:
    static std::unique_ptr<Logger>
    Create(const char* log_file_path)
    {
        std::cout << "Opening fd..." << std::endl;
        FILE* log_fd_ = fopen(log_file_path, "w");
        if(!log_fd_)
            return nullptr;
        std::cout << "Opened fd." << std::endl;
        return std::make_unique<Logger>(log_fd_);
    }

    Logger(FILE* log_fd_)
        : log_fd(log_fd_)
    {}

    ~Logger()
    {
        std::cout << "Closing fd..." << std::endl;
        fclose(log_fd);
        std::cout << "Done!" << std::endl;
    }

    void log(const char* msg)
    {
        uint16_t size = strlen(msg);
        std::cout << "Writing message..." << std::endl;
        if (fwrite(msg, sizeof(char), size, log_fd) < size)
        {
            std::cout << "Failed to log message" << std::endl;
        }
    }

private:
    FILE* log_fd;
};

int main(int argc, char** argv)
{
    auto l = Logger::Create("./file.txt");

    l->log("Here unique ptr 2\n");

    return 0;
}
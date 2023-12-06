#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

class Logger {
public:
    static void log(const std::string& message, int rank) {
        std::cout << std::endl << "Process rank: " << rank << " message: " << message << std::endl;
    }
};

#endif // LOGGER_H
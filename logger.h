#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger {
public:
    static Logger& instance();
    void log(const QString& message);

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H

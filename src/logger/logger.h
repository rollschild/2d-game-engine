#ifndef INCLUDED_LOGGER_H
#define INCLUDED_LOGGER_H

#include <string>
#include <vector>

enum LogType { LOG_INFO, LOG_WARNING, LOG_ERROR };

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
  public:
    static std::vector<LogEntry> messages;
    static void log(const std::string &message);
    static void err(const std::string &err_message);
};

#endif

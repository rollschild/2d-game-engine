#include "logger.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

std::vector<LogEntry> Logger::messages;

void Logger::log(const std::string &message) {
    auto sys_time = std::chrono::system_clock::now();
    LogEntry log_entry;
    log_entry.type = LOG_INFO;
    log_entry.message = std::format(
        "\033[32m{:%Y/%m/%d %H:%M:%S} LOG {}\033[0m\n", sys_time, message);
    // C++23 syntax
    // std::print(std::cout, "\033[32m{:%Y/%m/%d %H:%M:%S} LOG {}\033[0m\n",
    // sys_time, message);
    std::print(std::cout, "{}", log_entry.message);
    messages.push_back(log_entry);
}

void Logger::err(const std::string &err_message) {
    auto sys_time = std::chrono::system_clock::now();
    LogEntry log_entry;
    log_entry.type = LOG_ERROR;
    log_entry.message = std::format(
        "\033[31m{:%Y/%m/%d %H:%M:%S} ERR {}\033[0m\n", sys_time, err_message);
    std::print(std::cerr, "{}", log_entry.message);
    messages.push_back(log_entry);
}

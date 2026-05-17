#include "logger.h"

Logger::Logger()
{
    logFile.open("D:\\Projects\\City-Simulation\\logs\\log.txt", std::ios::app);
    if(!logFile.is_open()) throw std::runtime_error("Couldnt open log file!");
    logFile << "---New run---\n";
}

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::write(const std::string& msg)
{
    if(logFile.is_open())
    {
        logFile << msg << "\n";
        logFile.flush();
    }
}

ScopeTracer::ScopeTracer(const char* funcName):name(funcName)
{
    Logger::GetInstance().write("[ENTER] -> " + name);
}

ScopeTracer::~ScopeTracer()
{
    Logger::GetInstance().write("[EXIT] <- " + name);
}
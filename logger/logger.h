#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>

class Logger
{
public:
    static Logger& GetInstance();
    void write(const std::string& msg);
private:
    Logger(); 
private:
    std::ofstream logFile;
};

class ScopeTracer
{
public:
    ScopeTracer(const char* funcName);
    ~ScopeTracer();
    
private:
    std::string name;
};

#define AUTO_LOG() ScopeTracer tracer(__FUNCTION__)
#endif

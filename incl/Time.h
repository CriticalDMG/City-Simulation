#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include "helpers.h"
#include "types.h"

class Time
{
public:
    using clock = std::chrono::system_clock;

    static Time& obj();
    void advance(int days = 1);
    int GetDay() const;
    int GetPassedMonths(int startAbsoluteDay, int currAbsoluteDay) const;

    std::string GetCurrentTime() const 
    { 
        char* timeStr = std::ctime(&time);
        
        if(!timeStr)
        {
            LOG_ERROR(ERROR_CODES::INVALID_TIME_CONVERSION);
            throw (int)ERROR_CODES::INVALID_TIME_CONVERSION;
        }

        std::string str(timeStr);
        if (!str.empty() && str.back() == '\n') 
        {
            str.pop_back(); 
        }

        return str;
    }

    std::string getStartTime() const 
    { 
        std::string str = std::ctime(&startTime);
        str.pop_back();
        return str;
    }

private:
    Time();
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;

    std::time_t time;
    std::time_t startTime;
};
inline std::ostream& operator<<(std::ostream& os, const Time& t)
{
    os << t.GetCurrentTime();
    return os;
}

#endif //TIME_H
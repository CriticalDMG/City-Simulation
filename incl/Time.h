#ifndef TIME_H
#define TIME_H

#include <iostream>
#include <chrono>
#include <ctime>

class Time
{
public:
    using clock = std::chrono::system_clock;

    static Time& obj();
    void advance(size_t days = 1);
    int GetDay() const;

    const char* GetTime() const { return std::ctime(&time); }
    const char* getStartTime() const { return std::ctime(&startTime); }
private:
    Time();
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;

    std::time_t time;
    std::time_t startTime;
    
};

inline std::ostream& operator<<(std::ostream& os, const Time& t)
{
    os << t.GetTime();
    return os;
}

#endif //TIME_H
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
    int GetPassedMonths(int startAbsoluteDay, int currAbsoluteDay) const;

    std::string GetCurrentTime() const 
    { 
        std::string str = std::ctime(&time);
        str.pop_back();
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
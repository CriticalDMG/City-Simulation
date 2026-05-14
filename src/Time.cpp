#include "..\incl\Time.h"

Time::Time():time(clock::to_time_t(clock::now())), startTime(time) {}

void Time::advance(size_t days)
{
    auto timePoint = clock::from_time_t(time);

    auto future = timePoint + std::chrono::hours(days * 24);

    time = clock::to_time_t(future);
}

Time& Time::obj()
{
    static Time t;
    return t;
}

int Time::GetDay() const
{
    std::tm* timeStruct = std::localtime(&time);
    
    return timeStruct->tm_mday;
}

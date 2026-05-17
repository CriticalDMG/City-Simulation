#include "..\incl\Time.h"
#include "..\\Logger\\logger.h"

Time::Time():time(clock::to_time_t(clock::now())), startTime(time) { AUTO_LOG(); }

void Time::advance(size_t days)
{
    AUTO_LOG();
    auto timePoint = clock::from_time_t(time);

    auto future = timePoint + std::chrono::hours(days * 24);

    time = clock::to_time_t(future);
}

Time& Time::obj()
{
    AUTO_LOG();
    static Time t;
    return t;
}

int Time::GetDay() const
{
    AUTO_LOG();
    std::tm* timeStruct = std::localtime(&time);
    
    return timeStruct->tm_mday;
}

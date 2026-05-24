#include "..\incl\Time.h"
#include "..\\Logger\\logger.h"
#define SINGLEDAY 86400LL

Time::Time():time(clock::to_time_t(clock::now())), startTime(time) { AUTO_LOG(); }

void Time::advance(int days)
{
    AUTO_LOG();
    time += (days * SINGLEDAY);
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
    
    double diffInSeconds = std::difftime(time, startTime);
    
    int days = static_cast<int>(diffInSeconds / SINGLEDAY);
    
    return days + 1;
}

int Time::GetPassedMonths(int startCitDay, int currDay) const
{
    AUTO_LOG();
    
    std::time_t sTime = startTime + ((startCitDay - 1) * SINGLEDAY);
    std::time_t cTime = startTime + ((currDay - 1) * SINGLEDAY);

    std::tm* sTm = std::localtime(&sTime);
    int startMonth = sTm->tm_mon;
    int startYear = sTm->tm_year;
    int startDayOfMonth = sTm->tm_mday;

    std::tm* cTm = std::localtime(&cTime);
    int currMonth = cTm->tm_mon;
    int currYear = cTm->tm_year;

    int m = (currYear - startYear) * 12 + (currMonth - startMonth);
    
    if (startDayOfMonth == 1) {
        m++;
    }

    return m;
}
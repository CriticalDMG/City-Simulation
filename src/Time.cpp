#include "..\incl\Time.h"

Time::Time():time(clock::to_time_t(clock::now())), startTime(time) {}

void Time::advance(int days)
{
    time += (days * SINGLEDAY);
}

Time& Time::obj()
{
    static Time t;
    return t;
}

int Time::GetDay() const
{   
    double diffInSeconds = std::difftime(time, startTime);
    
    int days = static_cast<int>(diffInSeconds / SINGLEDAY);
    
    return days + 1;
}

int Time::GetPassedMonths(int startCitDay, int currDay) const
{   
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
    
    if(startDayOfMonth == 1) 
    {
        m++;
    }

    return m;
}
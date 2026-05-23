#include "..\\incl\\proffesion.h"
#include "..\\Logger\\logger.h"

#include <cstdlib>
#include <ctime>

int Teacher::CalcHappiness(int happ, int monthsAdv) const 
{
    AUTO_LOG();
    return std::min(100, happ + monthsAdv);
}

int Programmer::CalcHappiness(int happ, int monthsAdv) const
{
    AUTO_LOG();
    return std::max(0, happ - monthsAdv);
}

int Miner::CalcLife(int life, int monthsAdv) const
{
    AUTO_LOG();
    return std::max(0, life - (2 * monthsAdv));
} 

int Student::CalcHappiness(int happ, int monthsAdv) const
{
    AUTO_LOG();
    return std::max(0, happ - monthsAdv);
}

int Unemployed::CalcLife(int life, int monthsAdv) const
{
    AUTO_LOG();
    return std::max(0, life - monthsAdv);
}

int Unemployed::CalcHappiness(int happ, int monthsAdv) const 
{
    AUTO_LOG();
    return std::max(0, happ - monthsAdv);
}
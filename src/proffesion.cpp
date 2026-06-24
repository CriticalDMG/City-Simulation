#include "..\\incl\\proffesion.h"

#include <cstdlib>
#include <ctime>

//applies profession-specific formulas for stat degradation/growth over time
int Teacher::CalcHappiness(int happ, int monthsAdv) const 
{
    return std::min(100, happ + monthsAdv);
}

int Programmer::CalcHappiness(int happ, int monthsAdv) const
{
    return std::max(0, happ - monthsAdv);
}

int Miner::CalcLife(int life, int monthsAdv) const
{
    return std::max(0, life - (2 * monthsAdv));
} 

int Student::CalcHappiness(int happ, int monthsAdv) const
{
    return std::max(0, happ - monthsAdv);
}

int Unemployed::CalcLife(int life, int monthsAdv) const
{
    return std::max(0, life - monthsAdv);
}

int Unemployed::CalcHappiness(int happ, int monthsAdv) const 
{
    return std::max(0, happ - monthsAdv);
}
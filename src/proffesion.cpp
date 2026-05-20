#include "..\\incl\\proffesion.h"
#include "..\\Logger\\logger.h"

#include <cstdlib>
#include <ctime>

void Teacher::CalcHappiness(int& happ) const 
{
    AUTO_LOG();
    happ = std::max(happ + 1, 100);   
}

void Programmer::CalcHappiness(int& happ) const
{
    AUTO_LOG();
    happ = std::max(0, happ - 1);
}

void Miner::CalcLife(int& life) const
{
    AUTO_LOG();
    life = std::max(0, life - 2);
} 

void Student::CalcHappiness(int& happ) const
{
    AUTO_LOG();
    happ = std::max(0, happ - 1);
}

void Unemployed::CalcLife(int& life) const
{
    AUTO_LOG();
    life = std::max(0, life - 1);
}

void Unemployed::CalcHappiness(int& happ) const 
{
    AUTO_LOG();
    happ = std::max(0, happ - 1);
}
#include "..\\incl\\proffesion.h"
#include "..\\Logger\\logger.h"

#include <cstdlib>
#include <ctime>

unsigned int Proffesion::chooseSalary(int min, int max)
{
    AUTO_LOG();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    if (min > max) 
    {
        return 0; 
    }

    int salary = std::rand() % (min - max + 1) + min;
    
    return salary;
}

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

void Jobless::CalcLife(int& life) const
{
    AUTO_LOG();
    life = std::max(0, life - 1);
}

void Jobless::CalcHappiness(int& happ) const 
{
    AUTO_LOG();
    happ = std::max(0, happ - 1);
}
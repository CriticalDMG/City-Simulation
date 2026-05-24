#include "..\\incl\\citizen.h"
#include "..\\Logger\\logger.h"
#include <cstring>

Citizen::Citizen(std::string name, Proffesion* pr, 
                 int happiness, int life, unsigned int startMoney,
                 int creationDay, BuildingType type)
: happ(happiness), startHapp(happ),
  life(life), startLife(life),
  money(startMoney), startMoney(startMoney),
  creationDay(creationDay), proff(pr), name(std::move(name))
{
    AUTO_LOG();
    if(happiness < 0 || happiness > 100 || life < 0 || life > 100)
        throw std::invalid_argument("life and happiness should be between 0 and 100"); 
    

    if(type == DORM && proffesion() != STUDENT)
        throw std::invalid_argument("Citizen is not a student, so he cannot live in a dorm!"); 
}

void Citizen::updateStatistics(int currDay, int toPay)
{
    AUTO_LOG();

    int lived = currDay - creationDay;

    if(lived <= 0)
    {
        money = startMoney;
        life = startLife;
        happ = startHapp;
        return;
    }

    int month = Time::obj().GetPassedMonths(creationDay, currDay);

    long long virtualMoney = startMoney + month * salary() - (month * toPay) - (lived * FOOD);
    long long deficit = 0;

    if(virtualMoney <= 0)
    {
        money = 0;
        deficit = -virtualMoney;
    }
    else
    {
        money = virtualMoney;
    }

    int penalty = deficit / FOOD;

    int baseHappiness = proff->CalcHappiness(startHapp, month);
    int baseLife = proff->CalcLife(startLife, month);

    happ = std::max(0, baseHappiness - (penalty * 2));
    life = std::max(0, baseLife - (penalty * 1));

    if(life <= 0)
    {
        happ = 0;
        money = 0;
    }
}

std::ostream& operator<<(std::ostream& os, const Citizen& s)
{
    AUTO_LOG();
    os << "\n";
    os << "Name: " << s.GetName() << "\n"
       << "Life: " << s.GetLife() << "\n"
       << "Happiness: " << s.happiness() << "\n"
       << "Proffesion: " << s.proffesion() << "\n"
       << "Balance: " << s.balance() << "\n"; 
    return os;
}
#include "..\\incl\\citizen.h"
#include "..\\Logger\\logger.h"
#include <cstring>

Citizen::Citizen(std::string name, Proffesion* pr, 
                 int happiness, int life,BuildingType type)
:happ(happiness), life(life), 
 proff(pr), money(pr->salary()),
 name(std::move(name)) 
{
    AUTO_LOG();
    if(happiness < 0 || happiness > 100 || life < 0 || life > 100)
        throw std::invalid_argument("life and happiness should be between 0 and 100"); 
    

    if(type == DORM && proffesion() == STUDENT)
        throw std::invalid_argument("Citizen is not a student, so he cannot live in a dorm!"); 
}

void Citizen::simulate(int owe)
{
    AUTO_LOG();
    if(Time::obj().GetDay() == 1)
    {
        money += salary();
        proff->CalcHappiness(happ);
        proff->CalcLife(life);
        
        payRent(owe);
    }

    buyFood();
}

void Citizen::payRent(int owe)
{
    AUTO_LOG();
    if(money < owe)
    {
        happ = std::max(0, happ - 3);
        life = std::max(0, life - 2);
    }
    else
    {
        money -= owe;
    }
}

void Citizen::buyFood()
{
    AUTO_LOG();
    if(money < FOOD || proffesion() == UNEMPLOYED || proffesion() == STUDENT)
    {
        happ = std::max(0, happ - 2);
        life = std::max(0, life - 1);
        return;
    }

    money -= FOOD;
}

std::ostream& operator<<(std::ostream& os, const Citizen& s)
{
    AUTO_LOG();
    os << "\n";
    os << s.GetName() << " "
       << s.GetLife() << " "
       << s.happiness() << " "
       << s.proffesion() << " "
       << s.balance(); 
    return os;
}
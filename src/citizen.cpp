#include "..\\incl\\citizen.h"
#include "..\\Logger\\logger.h"
#include <cstring>

size_t Citizen::inst = 0;

Citizen::Citizen(std::string name, Proffesion* pr)
:happ(MAXHAPPINESS), life(MAXLIFE), 
 proff(pr), money(proff->salary()),
 name(std::move(name)) 
{
    AUTO_LOG();
    if(strcmp(proffesion(), "Dorm") == 0 && 
       strcmp(proffesion(), "Student") != 0)
            throw std::invalid_argument("Citizen is not a student, so he cannot live in a dorm!"); 
    
    incrementCounter();
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
    if(money < FOOD || strcmp(proffesion(), "Jobless") == 0 || strcmp(proffesion(), "Student") == 0)
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
#include "..\\incl\\citizen.h"
#include <cstring>

Citizen::Citizen(std::string name, Proffesion* pr, Building& b)
:happ(MAXHAPPINESS), life(MAXLIFE), 
 proff(pr), home(b), money(proff->salary()),
 name(std::move(name)) 
{
    if(strcmp(proffesion(), "Dorm") == 0 && 
       strcmp(proffesion(), "Student") != 0)
            throw std::invalid_argument("Citizen is not a student, so he cannot live in a dorm!"); 
}

void Citizen::simulate()
{
    if(Time::obj().GetDay() == 1)
    {
        money += salary();
        proff->CalcHappiness(happ);
        proff->CalcLife(life);
        
        payRent();
    }

    buyFood();
}

void Citizen::payRent()
{
    if(money < home.GetRent())
    {
        happ = std::max(0, happ - 3);
        life = std::max(0, life - 2);
    }
    else
    {
        money -= home.GetRent();
    }
}

void Citizen::buyFood()
{
    if(money < FOOD || strcmp(proffesion(), "Jobless") == 0 || strcmp(proffesion(), "Student") == 0)
    {
        happ = std::max(0, happ - 2);
        life = std::max(0, life - 1);
        return;
    }

    money -= FOOD;
}
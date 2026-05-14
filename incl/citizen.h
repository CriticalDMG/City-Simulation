#ifndef CITIZEN_H
#define CITIZEN_H

#define MAXLIFE 100
#define MAXHAPPINESS 100
#define FOOD 50

#include <string>
#include "proffesion.h"
#include "building.h"
#include "uniquePointer.h"
#include "Time.h"

class Citizen
{
public:
    Citizen(std::string name, Proffesion* pr, Building& b);

    void simulate();
    
    int happiness() const { return happ; }
    int GetLife() const { return life; }

    size_t cash() const { return money; }
    size_t salary() const { return proff->salary(); }

    const Building& GetHome() const { return home; }
    const char* proffesion() const  { return proff->GetProff(); }

    const std::string& GetName() const { return name; }

private:
    void payRent();
    void buyFood();

private:
    int happ;
    int life;

    uniquePointer<Proffesion> proff;
    Building& home;
    
    size_t money;
    std::string name;
};

#endif //CITIZEN_H
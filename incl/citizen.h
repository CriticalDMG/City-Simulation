#ifndef CITIZEN_H
#define CITIZEN_H
#define FOOD 50

#include <string>
#include "proffesion.h"
#include "uniquePointer.h"
#include "building.h"
#include "Time.h"

class Citizen
{
public:
    Citizen(std::string name, Proffesion* pr, int happiness, int life, BuildingType type);


    void payRent(int owe);
    void buyFood();
    void simulate(int owe);

    int happiness() const { return happ; }
    int GetLife() const { return life; }

    unsigned int balance() const { return money; }
    unsigned int salary() const { return proff->salary(); }

    ProffType proffesion() const  { return proff->GetProff(); }

    const std::string& GetName() const { return name; }

private:
    int happ;
    int life;

    uniquePointer<Proffesion> proff;
    
    unsigned int money;
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const Citizen& s);
#endif //CITIZEN_H
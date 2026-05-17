#ifndef CITIZEN_H
#define CITIZEN_H

#define MAXLIFE 100
#define MAXHAPPINESS 100
#define FOOD 50

#include <string>
#include "proffesion.h"
#include "uniquePointer.h"
#include "Time.h"

class Citizen
{
public:
    Citizen(std::string name, Proffesion* pr);

    void simulate(int owe);
    
    int happiness() const { return happ; }
    int GetLife() const { return life; }

    size_t balance() const { return money; }
    size_t salary() const { return proff->salary(); }

    const char* proffesion() const  { return proff->GetProff(); }

    const std::string& GetName() const { return name; }

private:
    static void incrementCounter();

    void payRent(int owe);
    void buyFood();

private:
    int happ;
    int life;

    uniquePointer<Proffesion> proff;
    
    static size_t inst;
    size_t money;
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const Citizen& s);
#endif //CITIZEN_H
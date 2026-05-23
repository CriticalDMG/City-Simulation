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
    Citizen(std::string name, Proffesion* pr,
            int happiness, int life, unsigned int startMoney,
            int creationDay, BuildingType type);

    void updateStatistics(int currDay, int toPay);

    unsigned int happiness() const { return happ; }
    unsigned int startappiness() const { return startHapp; }

    unsigned int GetLife() const { return life; }
    unsigned int StartLife() const { return startLife; }
 
    unsigned int balance() const { return money; }
    unsigned int salary() const { return proff->salary(); }
    unsigned int getStartMoney() const { return startMoney; }

    int creation() const { return creationDay; }
    ProffType proffesion() const  { return proff->GetProff(); }

    const std::string& GetName() const { return name; }

private:
    unsigned int happ        :7;
    unsigned int startHapp   :7;

    unsigned int life        :7;
    unsigned int startLife   :7;

    unsigned int money;
    unsigned int startMoney;

    int creationDay;

    uniquePointer<Proffesion> proff;
    
    std::string name;
};

std::ostream& operator<<(std::ostream& os, const Citizen& s);
#endif //CITIZEN_H
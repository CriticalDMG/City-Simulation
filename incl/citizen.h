#ifndef CITIZEN_H
#define CITIZEN_H

#include <string>
#include <iostream>
#include "time.h"
#include "types.h"
#include "BuildType.h"
#include "proffesion.h"

class Citizen
{    
public:
    Citizen(std::string name, const Proffesion* pr, 
                 int happiness, int life, unsigned int startMoney, 
                 int creationDay, unsigned int buildingId, BuildingType type);

    explicit Citizen(const CitizenPack& obj, 
                     std::string name, 
                     const Proffesion* proff,
                     int rent);

    explicit operator CitizenPack() const;

    void updateStatistics(int currDay, int rent);

    unsigned int GetSalary() const { return salary; }
    unsigned int balance() const { return money; }
    unsigned int GetStartMoney() const { return startMoney; }

    unsigned int happiness() const { return happ; }
    unsigned int startHappiness() const { return startHapp; }

    unsigned int GetLife() const { return life; }
    unsigned int StartLife() const { return startLife; }

    int creation() const { return creationDay; }
    ProffType proffesion() const  { return proff->GetProff(); }

    const std::string& GetName() const { return name; }

    bool operator==(const Citizen& oth) const;
    bool operator!=(const Citizen& oth) const;

private:
    int getDayOfDeath(int rent) const;
private:
    const Proffesion* proff;
    std::string name;

    unsigned int happ        :7;
    unsigned int startHapp   :7;

    unsigned int life        :7;
    unsigned int startLife   :7;

    unsigned int buildingId;

    unsigned int salary;
    unsigned int startMoney;
    unsigned int money;

    int creationDay;
};

std::ostream& operator<<(std::ostream& os, const Citizen& s);
#endif //CITIZEN_H
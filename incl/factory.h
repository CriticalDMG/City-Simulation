#ifndef FACTORY_H
#define FACTORY_H
#include "building.h"
#include "citizen.h"

class SimFactory
{
public:
    static Building createBuilding(int row, int col, BuildingType type, unsigned int MaxCit);
    static Citizen createCitizen(std::string name, ProffType proffType, 
                                 unsigned int salary, int happiness, int life, 
                                 unsigned int startMoney, int creationDay, 
                                 BuildingType bType);
};

#endif //FACTORY_H
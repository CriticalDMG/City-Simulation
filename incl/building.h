#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <iostream>
#include "types.h"
#include "BuildType.h"
#include "citizen.h"

class Building
{
public:
    Building(int row, int col, int matrixRows, int matrixCols, const BuildType* type, unsigned int max);    

    explicit Building(const BuildingPack& input, const BuildType* type);
    explicit operator BuildingPack() const;

    unsigned long long GetId() const { return id; }
    
    BuildingType GetType() const { return type->GetType(); }
    unsigned int GetRent() const { return rent; }

    unsigned int maxCitizens() const { return MaxCitCount; }

private:
    unsigned int id;
    
    const BuildType* type;
    //memory-optimized bit-fields to pack rent and capacity efficiently
    unsigned int rent           :16;
    unsigned int MaxCitCount    :16;
};

#endif //BUILDING_H
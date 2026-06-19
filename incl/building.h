#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include <iostream>
#include "types.h"
#include "BuildType.h"
#include "citizen.h"
#include "..\\Logger\\logger.h"

class Building
{
public:
    Building(int row, int col, int matrixRows, int matrixCols, const BuildType* type, unsigned int max);    

    explicit Building(const BuildingPack& input, const BuildType* type);
    explicit operator BuildingPack() const;

    unsigned long long GetId() const { return id; }
    
    BuildingType GetType() const { return type->GetType(); }
    unsigned int GetRent() const { return rent; }

    const std::vector<Citizen>& GetPpl() const { return ppl; }
    unsigned int maxCitizens() const { return MaxCitCount; }

    const Citizen& operator[](size_t index) const 
    {
        return ppl[index];
    }

    Citizen& operator[](size_t index) 
    {
        return ppl[index];
    }

private:
    unsigned int id;
    
    const BuildType* type;
    unsigned int rent           :16;

    unsigned int MaxCitCount    :16;
    std::vector<Citizen> ppl;
};

std::ostream& operator<<(std::ostream& os, const Building& obj);
#endif //BUILDING_H
#include "..\\incl\\helpers.h"
#include "..\\incl\\building.h"

Building::Building(int row, int col, int matrixRows, int matrixCols, 
                   const BuildType* type, unsigned int max)
: id(row * matrixCols + col), type(type), 
rent(type->calcRent(row, col, matrixRows, matrixCols)), 
MaxCitCount(max) {}

Building::Building(const BuildingPack& input, const BuildType* type)
:id(input.id), type(type), 
 rent(input.rent), MaxCitCount(input.maxCitCount) {}

Building::operator BuildingPack() const
{
    BuildingPack pack;

    pack.id = id;
    pack.rent = rent;
    pack.type = type->GetType();
    pack.maxCitCount = MaxCitCount;

    pack.LastResOffset = 0;

    return pack;
}
#include "..\incl\building.h"
#include "..\\Logger\\logger.h"
#include "..\\incl\\helpers.h"

Building::Building(int row, int col, int matrixRows, int matrixCols, 
                   const BuildType* type, unsigned int max)
: id(row * matrixCols + col), type(type), 
rent(type->calcRent(row, col, matrixRows, matrixCols)), 
MaxCitCount(max), ppl{} 
{ 
    AUTO_LOG(); 
}

Building::Building(const BuildingPack& input, const BuildType* type)
:id(input.id), type(type), 
 rent(input.rent), MaxCitCount(input.maxCitCount),
 ppl{} {}

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

std::ostream& operator<<(std::ostream& os, const Building& obj)
{
    AUTO_LOG();

    os << obj.GetRent() << " " 
       << obj.GetId() << " " 
       << obj.maxCitizens() << " "
       << helpers::ToBuildingType(obj.GetType());

    const std::vector<Citizen>& ppl = obj.GetPpl();
    for(size_t i = 0; i < ppl.size(); ++i)
    {
        os << ppl[i];
    }
    os << "\n";

    return os;
}
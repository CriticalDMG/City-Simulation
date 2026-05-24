#ifndef BUILDING_H
#define BUILDING_H
#include "BuildType.h"
#include "citizen.h"
#include "..\\Logger\\logger.h"
#include <vector>

class Citizen;
//class representing a building 
class Building
{
public:
    Building(int n, int m, BuildType* type, unsigned int max);
    
    /*
        other ctors not needed cause the compiler will generate will
        do the job. Copy operations will not be generated cause 
        uniquePointer has explicitly declared move semantics that
        stop copy semantics.
    */

    bool addPerson(Citizen person);
    void removePerson(const std::string& name);

    int row() const { return loc.row; }
    int col() const { return loc.col; }
    
    BuildingType GetType() const { return type->GetType(); }
    unsigned int GetRent() const { return rent; }

    const std::vector<Citizen>& GetPpl() const { return ppl; }
    unsigned int maxCitizens() const { return MaxCitCount; }

    const Citizen& operator[](size_t index) const {
        return ppl[index];
    }

    Citizen& operator[](size_t index) {
        return ppl[index];
    }

private:
    Location loc;
    uniquePointer<BuildType> type;
    unsigned int rent;

    unsigned int MaxCitCount;
    std::vector<Citizen> ppl;
};

std::ostream& operator<<(std::ostream& os, const Building& obj);

#endif //BUILDING_H
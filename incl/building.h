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
    Building(int n, int m, BuildType* type, size_t max);
    
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
    const BuildType* GetType() const { return type.get(); }
    size_t GetRent() const { return rent; }
    const std::vector<Citizen>& GetPpl() const { return ppl; }
    size_t max() const { return MaxCitCount; }

private:
    Location loc;
    uniquePointer<BuildType> type;
    size_t rent;

    size_t MaxCitCount;
    std::vector<Citizen> ppl;
};

inline std::ostream& operator<<(std::ostream& os, const Building& obj)
{
    AUTO_LOG();
    os << obj.GetRent() << " " 
       << obj.row() << " " 
       << obj.col() << " " 
       << obj.max() << " "
       << obj.GetType()->GetType();

    const std::vector<Citizen>& ppl = obj.GetPpl();
    for(size_t i = 0; i < ppl.size(); ++i)
    {
        os << ppl[i];
    }
    os << "\n";

    return os;
}

#endif //BUILDING_H
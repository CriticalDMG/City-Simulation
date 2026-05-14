#ifndef BUILDING_H
#define BUILDING_H
#include "BuildType.h"
#include <vector>

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

    int row() const { return loc.row; }
    int col() const { return loc.col; }
    const BuildType* GetType() const { return type.get(); }
    size_t GetRent() const { return rent; }
    size_t max() const { return MaxCitCount; }

private:
    Location loc;
    uniquePointer<BuildType> type;
    size_t rent;

    size_t MaxCitCount;
};

inline std::ostream& operator<<(std::ostream& os, const Building& obj)
{
    os << obj.GetRent() << " " 
       << obj.row() << " " 
       << obj.col() << " " 
       << obj.max() << " "
       << obj.GetType()->GetType();

    return os;
}

#endif //BUILDING_H
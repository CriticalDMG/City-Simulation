#ifndef BUILDING_H
#define BUILDING_H
#include "BuildType.h"
#include <vector>

class Building
{
public:
    Building(int n, int m, BuildType* type);

    Building(Building&& oth) noexcept;
    Building& operator=(Building&& oth) noexcept;

    ~Building() noexcept;
    
    int row() { return loc.row; }
    int col() { return loc.col; }
    BuildType* type() const { return tip; } 
    size_t GetRent() const { return rent; }

private:
    Location loc;
    BuildType* tip;
    size_t rent;
};

#endif //BUILDING_H
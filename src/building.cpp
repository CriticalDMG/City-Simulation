#include "..\incl\building.h"

Building::Building(int n, int m, BuildType* type)
:loc(n, m), tip(type)
{
    rent = tip->calcRent(loc, 100, 100);
}

Building::Building(Building&& oth) noexcept
:loc(oth.loc), tip(oth.tip), rent(oth.rent)
{
    oth.tip = nullptr;
    loc.row = -1;
    loc.col = -1;
}
    
Building& Building::operator=(Building&& oth) noexcept
{
    if(this != &oth)
    {
        delete tip;

        loc = oth.loc;
        tip = oth.tip;
        rent = oth.rent;

        oth.tip = nullptr;
        loc.row = -1;
        loc.col = -1;
    }   

    return *this;
}

Building::~Building() noexcept
{
    delete tip;
}
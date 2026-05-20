#ifndef BUILDTYPE_H
#define BUILDTYPE_H
#include "uniquePointer.h"

enum BuildingType
{
    UNKNOWN = -1,
    MODERN,
    PANEL,
    DORM
};

//packets the location of the building
struct Location
{
    int row;
    int col;

    Location(int n, int m): row(n),col(m) {}
};

/*
    This heirachy determines the type of the building(modern, panel, etc)
*/
class BuildType
{
public:
    virtual ~BuildType() = default;  
    BuildType(unsigned int rent);

    //location packets the coordinates of the building
    //rows and cols are the size of the matrix
    virtual unsigned int calcRent(const Location& loc, int rows, int cols) const;
    virtual BuildingType GetType() const = 0;
protected:
   

private:
    unsigned int baseRent;
};

class Modern: public BuildType
{
public:
    Modern(): BuildType(1000) {}
    virtual BuildingType GetType() const override { return MODERN; }
};

class Panel: public BuildType
{
public:
    Panel(): BuildType(500) {}
    virtual BuildingType GetType() const override { return PANEL; }
};

class Dorm: public BuildType
{
public:
    Dorm(): BuildType(67) {}
    virtual BuildingType GetType() const override { return DORM; }
};

#endif //BUILDTYPE_H

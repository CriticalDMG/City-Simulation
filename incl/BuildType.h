#ifndef BUILDTYPE_H
#define BUILDTYPE_H
#include "uniquePointer.h"

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
    BuildType(size_t rent);

    //location packets the coordinates of the building
    //rows and cols are the size of the matrix
    virtual size_t calcRent(const Location& loc, int rows, int cols) const;
    virtual const char* GetType() const = 0;
protected:
   

private:
    size_t baseRent;
};

class Modern: public BuildType
{
public:
    Modern(): BuildType(1000) {}
    virtual const char* GetType() const override { return "Modern"; }
};

class Panel: public BuildType
{
public:
    Panel(): BuildType(500) {}
    virtual const char* GetType() const override { return "Panel"; }
};

class Dorm: public BuildType
{
public:
    Dorm(): BuildType(67) {}
    virtual const char* GetType() const override { return "Dormitory"; }
};

#endif //BUILDTYPE_H

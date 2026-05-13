#ifndef BUILDTYPE_H
#define BUILDTYPE_H
#include <iostream>
struct Location
{
    int row;
    int col;

    Location(int n, int m): row(n),col(m) {}
};

//Тази йерархия определя типа на сградата
class BuildType
{
public:
    virtual ~BuildType() = default;  
    //location is the coordinates of the building
    //rows and cols are the size of the matrix
    virtual size_t calcRent(const Location& loc, int rows, int cols) const;
protected:
    BuildType(size_t rent);

private:
    size_t baseRent;
};

class Modern: public BuildType
{
public:
    Modern(): BuildType(1000) {}
};

class Panel: public BuildType
{
public:
    Panel(): BuildType(500) {}
};

class Dorm: public BuildType
{
public:
    Dorm(): BuildType(67) {}
};

#endif //BUILDTYPE_H

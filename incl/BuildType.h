#ifndef BUILDTYPE_H
#define BUILDTYPE_H
#include "helpers.h"
/*
    This heirachy determines the type of the building(modern, panel, etc)
*/

class BuildType
{
public:
    virtual ~BuildType() = default;

    virtual unsigned int getBaseRent() const = 0;
    //matrixRows and matrixCols are the size of the matrix
    //row and col represent the location the building will be placeds
    virtual unsigned int calcRent(int row, int col, int rows, int cols) const;    virtual BuildingType GetType() const = 0;
};

class Modern: public BuildType
{
public:
    virtual unsigned int getBaseRent() const override { return 1000; }
    virtual BuildingType GetType() const override { return MODERN; }
};

class Panel: public BuildType
{
public:
    virtual unsigned int getBaseRent() const override { return 500; }
    virtual BuildingType GetType() const override { return PANEL; }
};

class Dorm: public BuildType
{
public:
    virtual unsigned int getBaseRent() const override { return 67; }
    virtual BuildingType GetType() const override { return DORM; }
};

#endif //BUILDTYPE_H

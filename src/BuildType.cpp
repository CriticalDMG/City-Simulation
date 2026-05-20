#include "..\incl\BuildType.h"
#include "..\\Logger\\logger.h"
#include <cmath>

double dist(int x1, int y1, int x2, int y2)
{
    AUTO_LOG();
    return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

BuildType::BuildType(unsigned int rent): baseRent(rent) { AUTO_LOG(); }

unsigned int BuildType::calcRent(const Location& loc, int rows, int cols) const
{
    AUTO_LOG();
    double finalRent = this->baseRent;

    double distance = sqrt(dist(loc.row, loc.col, rows / 2, cols / 2));
    double min = std::min(rows, cols);

    if(distance <= min / 8.0)
    {
        finalRent *= 2.5;
    }
    else if(distance > 6.0 * min / 8.0)
    {
        finalRent *= 0.8;
    }

    return static_cast<unsigned int>(finalRent);
}


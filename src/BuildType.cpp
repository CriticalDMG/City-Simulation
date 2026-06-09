#include "..\incl\BuildType.h"
#include "..\\Logger\\logger.h"
#include <cmath>

double dist(int x1, int y1, int x2, int y2)
{
    AUTO_LOG();
    return ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

unsigned int BuildType::calcRent(int row, int col, int rows, int cols) const
{
    AUTO_LOG();
    double finalRent = this->getBaseRent();

    double centerX = rows / 2.0; 
    double centerY = cols / 2.0;

    double distance = sqrt(dist(centerX, centerY, row, col));    
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

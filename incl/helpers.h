#ifndef HELPERS_H
#define HELPERS_H
#include <fstream>
#include "types.h"
#include "citizen.h"


namespace helpers
{
    void logError(ERROR_CODES::Type code, const char* func);
    const char* convertCode(ERROR_CODES::Type code);
    bool verify(std::fstream& stream, unsigned char code);
    
    int predictDayOfDeath(const Citizen& citizen, int rent, int currentDay);

    int calcSalary(int min, int max);
    const char* ToProffession(ProffType);
    const char* ToBuildingType(BuildingType);
}

#define LOG_ERROR(code) helpers::logError(code, __FUNCTION__)
#endif //HELPERS_H
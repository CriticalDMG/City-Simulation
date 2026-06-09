#include "..\\incl\\helpers.h"
#include "..\\incl\\proffesion.h"
#include "..\\incl\\BuildType.h"
#include <cstdlib>
#include <ctime>

void helpers::logError(ERROR_CODES::Type code, const char* func)
{
    std::ofstream os("..\\logger\\error_log.txt");
    if(!os.is_open()) return;
    std::time_t now = std::time(0);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

    os << "[" << timestamp << "] "
              << "ERROR in " << func
              << "(Code: " << code << ") -> " 
              << helpers::convertCode(code) << '\n';

    os.close();
}

bool helpers::verify(std::fstream& stream, unsigned char code)
{
    stream.clear();
    stream.seekg(0, std::ios::beg);

    FileSignature check;
    stream.read(reinterpret_cast<char*>(&check), sizeof(check));

    if(check.sig != CITY_SIG || check.version != 1 || check.fileType != code)
    {
        return false;
    }

    return true;
}

const char* helpers::convertCode(ERROR_CODES::Type code)
{
    switch (code)
    {
        using namespace ERROR_CODES;

        case INVALID_TIME_CONVERSION: return "Invalid time conversion";
        
        case FILE_OPENING_FAILED: return "Opening file failed";

        case CITIZEN_FILE_CORRUPTED: return "citizens.bin has file has been corrupted";
    
        case CITIZEN_DATA_CORRUPTED: return "in citizens.bin data has been corrupted";

        case BUILDING_FILE_CORRUPTED: return "buildings.bin file has been corrupted";
        
        case BUILDING_DATA_CORRUPTED: return "buildings.bin data has been corrupted";
        
        case NAMES_FILE_CORRUPTED: return "names.bin has been corrupted";

        case INVALID_CHARACTERISTICS: return "Invalid characteristics detected";

        case INVALID_DORM_PLACEMENT: return "Invalid resident for dorm type";
        
        case BUILDING_CAPACITY_EXCEEDED: return "Building capacity exceeded";
        
        case DUPLICATE_CITIZEN_NAME: return "Duplicate citizen name in building";
        
        default: return "Unknown error code";
    }
    return nullptr;
}


int helpers::calcSalary(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
} 

const char* helpers::ToProffession(ProffType type)
{
    switch(type)
    {
        case TEACHER: return "Teacher";
        case PROGRAMMER: return "Programmer";
        case MINER: return "Miner";
        case STUDENT: return "Student";
        case UNEMPLOYED: return "Unemployed";         
    }
    
    return nullptr;
}

const char* helpers::ToBuildingType(BuildingType type)
{
    switch(type) 
    {
        case MODERN: return "Modern";
        case PANEL: return "Panel";
        case DORM: return "Dorm";
    }

    return nullptr;
}
#ifndef TYPES_H
#define TYPES_H
#include <iostream>
#include <ctime>

#define STATS_COUNT 3
#define CITY_SIG 0x43495459 //hex representation of 'CITY' in ASCII
#define SINGLEDAY 86400LL   //number of seconds in a standard 24-hour day
#define FOOD 50             //fixed daily food cost for all citizens

using uint64_t = unsigned long long;

namespace ERROR_CODES
{
    enum Type
    {
        INVALID_TIME_CONVERSION = -1,

        FILE_OPENING_FAILED = 1001,
        CITIZEN_FILE_CORRUPTED = 1002,
        CITIZEN_DATA_CORRUPTED = 1003,
        BUILDING_FILE_CORRUPTED = 1004,
        BUILDING_DATA_CORRUPTED = 1005,
        NAMES_FILE_CORRUPTED = 1006,
        SAVE_FILE_CORRUPTION = 1007,

        INVALID_CHARACTERISTICS = 2001,
        INVALID_DORM_PLACEMENT = 2002,

        BUILDING_CAPACITY_EXCEEDED = 3001,
        DUPLICATE_CITIZEN_NAME = 3002
    };
}

enum ProffType
{
    PROFFTYPE_UNKNOWN = -1,
    
    TEACHER,
    PROGRAMMER,
    MINER,
    STUDENT,
    UNEMPLOYED,
    
    PROFFESION_COUNTER
};

enum BuildingType
{
    BUILDINGTYPE_UNKNOWN = -1,
    
    MODERN,
    PANEL,
    DORM,

    TYPE_COUNTER
};

//holds statistical tracking data during time advancement steps
struct StepStats
{
    int removed{};
    int emptyHapp{};
    int broke{};
};

//aggregated simulation statistics for all living citizens
struct CitizenStats
{
    unsigned int happ{};
    long long money{};
    unsigned int life{};

    int aliveCit{};
    int proffStats[ProffType::PROFFESION_COUNTER]{}; 
    // 0 - Teacher, 1 - Programmer
    // 2 - Miner, 3 - Student 4 - Unemployed

    int min[STATS_COUNT]{INT_MAX, INT_MAX, INT_MAX}; // 0 - happ, 1 - money, 2 - life
    int max[STATS_COUNT]{}; // 0 - happ, 1 - money, 2 - life
};

//aggregated simulation statistics for all constructed buildings
struct BuildingStats
{
    int type[BuildingType::TYPE_COUNTER]{}; //0 - Modern, 1 - Panel, 2 - Dorm
};

struct Identificator
{
    uint64_t id{};
    uint64_t fileOffset{};

    unsigned int currLiving{};
    unsigned int capacity{};
};

#pragma pack(push, 1)
    struct FileSignature
    {
        int sig{CITY_SIG};

        unsigned short version{1};
        
        unsigned char fileType{}; //0 - save file, 1 - config file
        
        FileSignature() {}

        FileSignature(unsigned char type): fileType{type} {};
    };
    struct CitizenPack
    {
        unsigned int signature{CITY_SIG};

        unsigned int startHapp :7 {};
        unsigned int startLife :7 {};
        unsigned int proff     :3 {};
        unsigned int salary    :15{};

        int nameLen{};
        int startMoney{};

        int creationDay{};
        int remDay{}; //flags the exact day of death or -1 if the citizen is alive

        unsigned int buildingId{};

        uint64_t prevCitOffset{}; //offset to the previously added citizen in the same building
        uint64_t nameOffset{};    //offset to the actual string data in names.bin
    };

    struct BuildingPack
    {
        unsigned int signature{CITY_SIG};
        unsigned int id{};

        unsigned int rent          :14{};
        unsigned int maxCitCount   :14{};
        unsigned int type          :4 {};

        uint64_t LastResOffset{}; //offset to the most recently added citizen in this building
    };

#pragma pack(pop)

#endif //TYPES_H
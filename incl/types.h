#ifndef TYPES_H
#define TYPES_H
#include <iostream>
#include <ctime>

#define STATS_COUNT 3
#define CITY_SIG 0x43495459
#define SINGLEDAY 86400LL
#define FOOD 50

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
    BUILDIGNTYPE_UNKNOWN = -1,
    
    MODERN,
    PANEL,
    DORM,

    TYPE_COUNTER
};

struct StepStats
{
    int removed{};
    int emptyHapp{};
    int broke{};
};

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

struct BuildingStats
{
    int type[BuildingType::TYPE_COUNTER]{}; //0 - Modern, 1 - Panel, 2 - Dorm
};

#pragma pack(push, 1)
    struct FileSignature
    {
        int sig{CITY_SIG};

        unsigned short version{1};
        
        unsigned char fileType{}; //0 - save file, 1 - citizens.bin file, 2 - names.bin, 3 - buildings.bin
        
        FileSignature() {}

        FileSignature(unsigned char type): fileType{type} {};
    };

    struct SaveHeader
    {
        FileSignature sig;

        int day;

        int rows;
        int cols;

        int citizenCount;
        int buildingsCount;
    };

    struct CitizenPack
    {
        unsigned int signature{CITY_SIG};

        int startHapp :7 {};
        int startLife :7 {};
        int proff     :3 {};
        int salary    :15{};

        int nameLen{};
        int startMoney{};

        int creationDay{};
        int remDay{};

        unsigned int buildingId{};

        uint64_t prevCitOffset{};
        uint64_t nameOffset{};
    };

    struct BuildingPack
    {
        unsigned int signature{CITY_SIG};
        unsigned int id{};

        int rent          :14{};
        int maxCitCount   :14{};
        int type          :4 {};

        uint64_t LastResOffset{};
    };

#pragma pack(pop)

#endif //TYPES_H
#ifndef FILE_ENGINE_H
#define FILE_ENGINE_H
#include "CitizenRepo.h"
#include "BuildingRepo.h"
#include "Register.h"


//the core orchestrator of the simulation 
//manages the chronological advancement, temporal state, 
//and data synchronization between Repositories
class FileEngine
{
public:
    FileEngine(int r, int c);
    FileEngine(const char* loadFilename);

    //serialization and deserialization main endpoints
    bool save(const char* filename);
    bool load(const char* filename);
    
    //Core Simulation Interface    
    bool addBuildingTo(int row, int col, const BuildType* type, unsigned int maxCapacity);
    bool addCitizenTo(int row, int col, const char* name, const Proffesion* job, int happ, int money, int life);
    bool removeCitizenFrom(int row, int col, const char* name);
    StepStats advanceTime(int days);
    
    int getCurrentDay() const { return currentDay; }

    std::vector<std::string> getGlobalInfo();
    std::vector<std::string> getBuildingInfo(int row, int col);
    std::vector<std::string> getCitizenInfo(int row, int col, const char* name);

    const CitizenStats& getCitizenStats() const { return cStats; }
    const BuildingStats& getBuildingStats() const { return bStats; }

private:
    //returns the identificator of the building
    Identificator* findByID(uint64_t id);

    //lower-bound binary search algorithm 
    //finds the exact insertion index required to 
    //maintain sorted order in the vector
    int getPos(uint64_t id);

    //future Lookahead algorithm. Prevents the "Temporal Paradox" 
    //where adding a citizen in the past overflows building capacity in the future
    int getFuturePeakOccupancy(const BuildingPack& bPack);
private:
    CitizenRepo citizenRepo;
    BuildingRepo buildingRepo;
    std::vector<Identificator> info;

    ProffRegister proffReg;
    BuildingRegister buildReg;

    int currentDay;
    int matrixRows;
    int matrixCols;

    CitizenStats cStats;
    BuildingStats bStats;
};

#endif
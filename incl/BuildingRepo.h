#ifndef BUILDING_REPO_H
#define BUILDING_REPO_H
#include "BuildType.h"
#include "helpers.h"
#include <vector>
#include <fstream>

class FileEngine;
class CitizenRepo;

class BuildingRepo
{
public:
    friend class FileEngine;

    ~BuildingRepo() noexcept;

    BuildingPack getBulding(const Identificator& identificator);
    void saveChanges(const BuildingPack& pack, const Identificator& identificator);

    bool save(std::ofstream& out, BuildingPack& outPack);
    bool readFromSave(std::ifstream& in, BuildingPack& pack, int& totalPeople);
    
private:
    BuildingRepo();
    Identificator addBuilding(unsigned int id, const BuildType* type, 
                         int row, int col, int matrixRows, int matrixCols, 
                         unsigned int maxCapacity);
    BuildingRepo(BuildingRepo&&) noexcept;

    void resetReadSequence();
    uint64_t encodeBuilding(const BuildingPack& pack);
private:
    static constexpr const char* BUILDINGS_FILENAME = "..\\files\\buildings.bin";

    std::fstream buildings;
};

#endif //BUILDING_REPO_H
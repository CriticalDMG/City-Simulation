#ifndef BUILDING_REPO_H
#define BUILDING_REPO_H
#include "BuildType.h"
#include <vector>

class FileEngine;

struct buildingLocation
{
    uint64_t id{};
    uint64_t fileOffset{};

    unsigned int currLiving{};
    unsigned int capacity{};
};

class BuildingRepo
{
public:
    friend class FileEngine;
    ~BuildingRepo() noexcept;

    BuildingPack& getBulding(uint64_t);
    void saveChanges(const BuildingPack& pack);

    bool readBuildng(BuildingPack& pack);
    bool saveBuilding(const BuildingPack& pack);
    
private:
    BuildingRepo();
    BuildingRepo(BuildingRepo&&) noexcept;

    uint64_t findByID(uint64_t id);
private:
    static constexpr const char* BUILDINGS_FILENAME = "..\\files\\buildings.bin";

    std::fstream buildings;
    std::vector<buildingLocation> info;
};

#endif //BUILDING_REPO_H
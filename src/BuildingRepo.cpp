#include "..\\incl\\BuildingRepo.h"
#include "..\\incl\\building.h"

Identificator BuildingRepo::addBuilding(unsigned int id, const BuildType* type, 
                                   int row, int col, int matrixRows, int matrixCols, 
                                   unsigned int maxCapacity)
{
    BuildingPack pack;

    try
    {
        Building b(row, col, matrixRows, matrixCols, type, maxCapacity);
        
        pack = (BuildingPack)b; 
        
        pack.id = id; 
        pack.LastResOffset = 0; 
    }catch(int code) 
    {
        std::cout << "Building not created with error code: " << code << "\n";
        return Identificator{};
    }

    uint64_t offset = encodeBuilding(pack);

    Identificator ident;
    ident.id = id;
    ident.fileOffset = offset;
    ident.currLiving = 0;
    ident.capacity = maxCapacity;

    return ident;
}

BuildingRepo::BuildingRepo()
:buildings(BUILDINGS_FILENAME, std::ios::in | std::ios::out | std::ios::binary)
{
    if(!buildings.is_open())
    {
        LOG_ERROR(ERROR_CODES::FILE_OPENING_FAILED);
        throw (int)ERROR_CODES::FILE_OPENING_FAILED;
    }

    FileSignature sig(1);

    buildings.write(reinterpret_cast<const char*>(&sig), sizeof(sig));
}

BuildingRepo::~BuildingRepo() noexcept
{
    buildings.close();
    std::ofstream out(BUILDINGS_FILENAME, std::ios::trunc);
    out.close();    
}

void BuildingRepo::saveChanges(const BuildingPack& pack, const Identificator& identificator)
{
    buildings.clear();
    buildings.seekp(identificator.fileOffset, std::ios::beg); // SEEKP (за писане!)
    buildings.write(reinterpret_cast<const char*>(&pack), sizeof(pack));
}

BuildingPack BuildingRepo::getBulding(const Identificator& identificator)
{
    BuildingPack p;
    buildings.clear();
    buildings.seekg(identificator.fileOffset, std::ios::beg);

    if(!buildings.read(reinterpret_cast<char*>(&p), sizeof(p)))
    {
        return BuildingPack{};
    }
    return p;
}

void BuildingRepo::resetReadSequence()
{
    buildings.clear();
        
    buildings.seekg(sizeof(FileSignature), std::ios::beg); 
}

bool BuildingRepo::save(std::ofstream& out, BuildingPack& outPack)
{
    if(!out.good()) return false;

    if(!buildings.read(reinterpret_cast<char*>(&outPack), sizeof(outPack)) || outPack.signature != CITY_SIG)
    {
        return false;
    }

    out.write(reinterpret_cast<const char*>(&outPack), sizeof(outPack));

    int zeroPlaceholder = 0;
    out.write(reinterpret_cast<const char*>(&zeroPlaceholder), sizeof(zeroPlaceholder));

    return true;
}

bool BuildingRepo::readFromSave(std::ifstream& in, BuildingPack& pack, int& totalPeople)
{
    if(!in.read(reinterpret_cast<char*>(&pack), sizeof(pack)))
    {
        return false; 
    }

    if(pack.signature != CITY_SIG)
    {
        LOG_ERROR(ERROR_CODES::BUILDING_DATA_CORRUPTED);
        return false;
    }

    if(!in.read(reinterpret_cast<char*>(&totalPeople), sizeof(totalPeople)))
    {
        return false;
    }

    return true;
}

uint64_t BuildingRepo::encodeBuilding(const BuildingPack& pack)
{
    buildings.clear();
    buildings.seekp(0, std::ios::end); 
    
    uint64_t offset = buildings.tellp();

    buildings.write(reinterpret_cast<const char*>(&pack), sizeof(pack));

    return offset;
}

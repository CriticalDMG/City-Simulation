#include "..\\incl\\CitizenRepo.h"
#include "..\\incl\\citizen.h"

CitizenRepo::CitizenRepo()
:citizens(CITIZEN_FILENAME, std::ios::in | std::ios::out | std::ios::trunc),
names(NAMES_FILENAME, std::ios::in | std::ios::out | std::ios::trunc)
{
    if(!citizens.is_open() || !names.is_open())
    {
        LOG_ERROR(ERROR_CODES::FILE_OPENING_FAILED);
        throw (int)ERROR_CODES::FILE_OPENING_FAILED;
    }

    FileSignature citizenSig(1);

    citizens.write(reinterpret_cast<const char*>(&citizenSig), sizeof(citizenSig));
}

CitizenRepo::~CitizenRepo() noexcept
{
    if(citizens.is_open())
    {
        if(!citizens.good())
        {
            citizens.clear();
            LOG_ERROR(ERROR_CODES::CITIZEN_FILE_CORRUPTED);
        }

        citizens.close();
    }   

    if(names.is_open())
    {
        if(!names.good())
        {
            names.clear();
            LOG_ERROR(ERROR_CODES::NAMES_FILE_CORRUPTED);
        }

        names.close();
    }   

    std::ofstream truncCitizens(CITIZEN_FILENAME, std::ios::trunc);
    std::ofstream truncNames(NAMES_FILENAME, std::ios::trunc);

    truncCitizens.close();
}

uint64_t CitizenRepo::addCitizen(const char* name, const Proffesion* ptr, 
                    int happ, int money, int life,
                    size_t lastCitOff, unsigned int id, 
                    BuildingType bType, int currDay, int rent)
{
    if(!name || !name[0]) return 0;
    CitizenPack pack;
    size_t len = 0;     
    
    try{
        Citizen cit(name, ptr, happ, life, money, currDay, id, bType);
        pack = (CitizenPack)cit;
        len = cit.GetName().size();
        pack.remDay = helpers::predictDayOfDeath(cit, rent, currDay);
    }catch(int code){
        std::cout << "Citizen not created with error code: " << code << "\n";
        return 0;
    }

    pack.nameOffset = encodeName(name, len);
    pack.prevCitOffset = lastCitOff;

    return encodeCitizen(pack);
}

int CitizenRepo::readCitizenAt(uint64_t offset, CitizenPack& out)
{
    if(!citizens.good()) 
    { 
        citizens.clear(); 
    };

    citizens.seekg(offset, std::ios::beg);

    if(!citizens.read(reinterpret_cast<char*>(&out), sizeof(out)))
    {
        LOG_ERROR(ERROR_CODES::CITIZEN_FILE_CORRUPTED);
        return ERROR_CODES::CITIZEN_FILE_CORRUPTED;
    }

    if(out.signature != CITY_SIG)
    {
        LOG_ERROR(ERROR_CODES::CITIZEN_DATA_CORRUPTED);
        return ERROR_CODES::CITIZEN_DATA_CORRUPTED;
    }

    return 1;
}

int CitizenRepo::removeCitizen(uint64_t offset, int currDay)
{
    if(!citizens.good()) 
    {
        citizens.clear();
    }

    CitizenPack pack;

    citizens.seekg(offset, std::ios::end);

    if(!citizens.read(reinterpret_cast<char*>(&pack), sizeof(pack)))
    {
        LOG_ERROR(ERROR_CODES::CITIZEN_FILE_CORRUPTED);
        return 0;
    }

    if(pack.signature != CITY_SIG)
    {
        LOG_ERROR(ERROR_CODES::CITIZEN_DATA_CORRUPTED);
        return 0;
    }

    if(pack.creationDay <= currDay && pack.remDay > currDay)
    {
        pack.remDay = currDay;
        return 1;
    }

    return 0;
}   

int CitizenRepo::loadCitizens(std::ifstream& in, BuildingRepo& repo, int totalPeople)
{
    CitizenPack pack;

    for(int i = 0; i < totalPeople; ++i)
    {
        if(!in.read(reinterpret_cast<char*>(&pack), sizeof(pack)))
        {
            LOG_ERROR(ERROR_CODES::SAVE_FILE_CORRUPTION);
            return 0;
        }

        if(pack.signature == CITY_SIG)
        {
            BuildingPack& building = repo.getBulding(pack.buildingId);
            pack.prevCitOffset = building.LastResOffset;

            char buff[512];
            if(!in.read(reinterpret_cast<char*>(buff), pack.nameLen))
            {   
                LOG_ERROR(ERROR_CODES::SAVE_FILE_CORRUPTION);
                return 0;
            }

            pack.nameOffset = encodeName(buff, pack.nameLen);
            uint64_t newCitOffset = encodeCitizen(pack);
        
            building.LastResOffset = newCitOffset;    
            repo.saveChanges(building);
        }   
    }

    return 1;
}

int CitizenRepo::saveCitizens(std::ofstream& out)
{
    citizens.clear();
    names.clear();

    citizens.seekg(0, std::ios::beg);
    names.seekg(0, std::ios::beg);

    CitizenPack cit;

    while(citizens.read(reinterpret_cast<char*>(&cit), sizeof(cit)))
    {
        if(cit.signature == CITY_SIG)
        {
            out.write(reinterpret_cast<const char*>(&cit), sizeof(cit));
            
            char buff[512];
            names.seekg(cit.nameOffset, std::ios::beg);

            if(!names.read(buff, cit.nameLen))
            {
                return 0;
            }

            out.write(buff, cit.nameLen + 1);
        }
    }   

    return 1;
}

CitizenRepo::Iterator::Iterator(std::fstream& stream, int day, bool flag)
:file(stream), currPack{}, end(flag), currDay(day)
{
    if(!end)
    {
        readNextAlive();
    }
}   

CitizenRepo::Iterator& CitizenRepo::Iterator::operator++()
{
    readNextAlive();
    return *this;
}

bool CitizenRepo::Iterator::operator!=(const Iterator& oth) const
{
    return this->end != oth.end;
}

bool CitizenRepo::Iterator::operator==(const Iterator& oth) const
{
    return !(*this != oth);
}   

void CitizenRepo::Iterator::readNextAlive()
{
    while(file.read(reinterpret_cast<char*>(&currPack), sizeof(currPack)))
    {
        if(currPack.signature != CITY_SIG)
        {
            LOG_ERROR(ERROR_CODES::CITIZEN_DATA_CORRUPTED);
            throw (int)ERROR_CODES::CITIZEN_DATA_CORRUPTED;
        }

        if(currPack.remDay == -1 || currPack.remDay > currDay)
        {
            return;
        }
    }

    if(file.eof()) 
    {
        end = true;
        return;
    }

    if(file.fail() || file.bad())
    {
        LOG_ERROR(ERROR_CODES::CITIZEN_FILE_CORRUPTED);
        throw (int)ERROR_CODES::CITIZEN_FILE_CORRUPTED;
    }
}

CitizenRepo::Iterator CitizenRepo::begin(int currDay)
{
    if(!helpers::verify(citizens, 1))
    {
        LOG_ERROR(ERROR_CODES::CITIZEN_FILE_CORRUPTED);
        throw (int)ERROR_CODES::CITIZEN_FILE_CORRUPTED;
    }

    return Iterator(citizens, currDay);
}

CitizenRepo::Iterator CitizenRepo::end()
{
    return Iterator(citizens, 0, true);
}

uint64_t CitizenRepo::encodeName(const char* str, size_t size)
{
    uint64_t offset = names.tellp();
    names.write(str, size);

    return offset;
}

uint64_t CitizenRepo::encodeCitizen(CitizenPack& cit)
{
    uint64_t offset = citizens.tellp();
    citizens.write(reinterpret_cast<char*>(&cit), sizeof(cit));

    return offset;
}


#include "..\\incl\\CitizenRepo.h"
#include "..\\incl\\citizen.h"
#include <cstring>

CitizenRepo::CitizenRepo()
:citizens(CITIZEN_FILENAME, std::ios::in | std::ios::out | std::ios::binary),
names(NAMES_FILENAME, std::ios::in | std::ios::out | std::ios::binary)
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
    citizens.close();
    names.close();

    std::ofstream truncCitizens(CITIZEN_FILENAME, std::ios::trunc);
    std::ofstream truncNames(NAMES_FILENAME, std::ios::trunc);

    truncCitizens.close();
    truncNames.close();
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

int CitizenRepo::removeCitizen(uint64_t lastResOffset, const char* targetName, int currDay)
{
    if(!citizens.good()) citizens.clear();
    if(!names.good()) names.clear();

    uint64_t currOffset = lastResOffset;

    while(currOffset != 0)
    {
        CitizenPack pack;
        
        citizens.seekg(currOffset, std::ios::beg); 
        
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

        if((pack.creationDay <= currDay && (pack.remDay == -1 || currDay < pack.remDay)))
        {
            char buff[512];
            names.seekg(pack.nameOffset, std::ios::beg);
            names.read(buff, pack.nameLen);
            buff[pack.nameLen] = '\0';
            if(strcmp(buff, targetName) == 0)
            {
                pack.remDay = currDay;
                
                citizens.seekp(currOffset, std::ios::beg);
                citizens.write(reinterpret_cast<const char*>(&pack), sizeof(pack));
                
                citizens.flush();
                
                return 1;
            }
        }

        currOffset = pack.prevCitOffset;
    }

    return 0;
}

BuildingPack& CitizenRepo::loadCitizens(std::ifstream& in, BuildingPack& building, int totalPeople)
{
    building.LastResOffset = 0;
    
    struct TempCit 
    {
        CitizenPack pack;
        std::string name;
    };
    
    std::vector<TempCit> temp;

    for(int i = 0; i < totalPeople; ++i)
    {
        CitizenPack pack;
        if(!in.read(reinterpret_cast<char*>(&pack), sizeof(pack)))
            throw (int)ERROR_CODES::SAVE_FILE_CORRUPTION;

        if(pack.signature == CITY_SIG)
        {
            char buff[512];
            if(!in.read(reinterpret_cast<char*>(buff), pack.nameLen))
                throw (int)ERROR_CODES::SAVE_FILE_CORRUPTION;
            buff[pack.nameLen] = '\0';

            temp.push_back({pack, std::string(buff)});
        }   
    }

    for(int i = (int)temp.size() - 1; i >= 0; --i)
    {
        temp[i].pack.prevCitOffset = building.LastResOffset;
        temp[i].pack.nameOffset = encodeName(temp[i].name.c_str(), temp[i].pack.nameLen);
        building.LastResOffset = encodeCitizen(temp[i].pack);
    }

    return building;
}

int CitizenRepo::saveCitizens(std::ofstream& out, uint64_t firstCitizenOffset)
{
    if (firstCitizenOffset == 0) return 0;

    citizens.clear();
    names.clear();

    int totalSaved = 0;

    citizens.seekg(firstCitizenOffset, std::ios::beg);
    names.seekg(0, std::ios::beg);

    CitizenPack cit;

    while(citizens.read(reinterpret_cast<char*>(&cit), sizeof(cit)))
    {
        if(cit.signature != CITY_SIG) return -1;

        out.write(reinterpret_cast<const char*>(&cit), sizeof(cit));
        char buff[512];
        names.seekg(cit.nameOffset, std::ios::beg);

        if(!names.read(buff, cit.nameLen)) return -1;
        
        out.write(buff, cit.nameLen);
        ++totalSaved;
        
        uint64_t nextOffset = cit.prevCitOffset;
        if(nextOffset == 0) break;

        citizens.seekg(nextOffset, std::ios::beg);
    }

    return totalSaved;
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
    names.clear();
    names.seekp(0, std::ios::end);
    uint64_t offset = names.tellp();
    names.write(str, size);
    return offset;
}

uint64_t CitizenRepo::encodeCitizen(CitizenPack& cit)
{
    citizens.clear();
    citizens.seekp(0, std::ios::end);
    uint64_t offset = citizens.tellp();
    citizens.write(reinterpret_cast<char*>(&cit), sizeof(cit));
    return offset;
}

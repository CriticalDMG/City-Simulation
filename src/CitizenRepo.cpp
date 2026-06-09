#include "..\\incl\\CitizenRepo.h"
#include "..\\incl\\helpers.h"

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
    FileSignature namesSig(2);

    citizens.write(reinterpret_cast<const char*>(&citizenSig), sizeof(citizenSig));
    names.write(reinterpret_cast<const char*>(&namesSig), sizeof(namesSig));
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

    std::ofstream wipe_citizens(CITIZEN_FILENAME, std::ios::trunc);
    std::ofstream wipe_names(NAMES_FILENAME, std::ios::trunc);
}

size_t CitizenRepo::addCitizen(const char* name, ProffType type, 
                    int happ, int money, int life,
                    size_t lastCitOff, unsigned int id, 
                    BuildingType bType, int currDay)
{
    return 0;
}

bool CitizenRepo::readCitizenAt(size_t offset, CitizenPack& out)
{
    return true;   
}

bool CitizenRepo::removeCitizen(size_t offset, int currDay)
{
    return true;
}   

bool CitizenRepo::loadCitizens(std::ifstream& in)
{
    return true;
}

bool CitizenRepo::saveCitizens(std::ofstream& out)
{
    return true;
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
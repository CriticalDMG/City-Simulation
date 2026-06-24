#ifndef CITIZEN_REPO_H
#define CITIZEN_REPO_H
#include <fstream>
#include "BuildingRepo.h"
#include "proffesion.h"

//handles direct binary disk I/O operations for Citizens and their Names
class CitizenRepo
{
public:
    friend class FileEngine;
    ~CitizenRepo() noexcept;
    
    int readCitizenAt(uint64_t offset, CitizenPack& out);
    int removeCitizen(uint64_t lastResOffset, const char* targetName, int currDay);
    
    BuildingPack& loadCitizens(std::ifstream& in, BuildingPack& building, int totalPeople);
    int saveCitizens(std::ofstream& out,  uint64_t firstCitizenOffset);

    //custom Forward Iterator allowing seamless iteration over ALIVE citizens 
    //directly from the binary file without loading the entire population into RAM
    class Iterator
    {
    public:
        friend class CitizenRepo;
        const CitizenPack& operator*() { return currPack; }

        Iterator& operator++();
        bool operator!=(const Iterator& oth) const;
        bool operator==(const Iterator& oth) const;
    
    private:
        void readNextAlive();    
        Iterator(std::fstream& stream, int day, bool flag = false);
        
    private:
        std::fstream& file;
        CitizenPack currPack;
        bool end;
        int currDay;
    };

    Iterator begin(int currDay);
    Iterator end();
private:
    CitizenRepo();
    CitizenRepo(CitizenRepo&&) noexcept;
    uint64_t addCitizen(const char* name, const Proffesion* ptr, 
                    int happ, int money, int life,
                    uint64_t lastCitOff, unsigned int id, 
                    BuildingType bType, int currDay, int rent);

    uint64_t encodeName(const char* str, size_t size);
    uint64_t encodeCitizen(CitizenPack& cit);

private:
    static constexpr const char* CITIZEN_FILENAME = "..\\files\\citizen.bin";
    static constexpr const char* NAMES_FILENAME = "..\\files\\names.bin";

    std::fstream citizens;
    std::fstream names;
};

#endif //CITIZEN_REPO_H
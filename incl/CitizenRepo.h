#ifndef CITIZEN_REPO_H
#define CITIZEN_REPO_H
#include <fstream>
#include "BuildingRepo.h"
#include "proffesion.h"

class CitizenRepo
{
public:
    friend class FileEngine;
    ~CitizenRepo() noexcept;

    size_t addCitizen(const char* name, const Proffesion* ptr, 
                    int happ, int money, int life,
                    size_t lastCitOff, unsigned int id, 
                    BuildingType bType, int currDay, int rent);
    
    int readCitizenAt(uint64_t offset, CitizenPack& out);
    int removeCitizen(uint64_t offset, int currDay);
    
    int loadCitizens(std::ifstream& in, BuildingRepo& repo, int totalPeople);
    int saveCitizens(std::ofstream& out);

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

    uint64_t encodeName(const char* str, size_t size);
    uint64_t encodeCitizen(CitizenPack& cit);

private:
    static constexpr const char* CITIZEN_FILENAME = "..\\files\\citizen.bin";
    static constexpr const char* NAMES_FILENAME = "..\\files\\names.bin";

    std::fstream citizens;
    std::fstream names;
};

#endif //CITIZEN_REPO_H
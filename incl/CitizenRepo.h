#ifndef CITIZEN_REPO_H
#define CITIZEN_REPO_H
#include "citizen.h"

class CitizenRepo
{
public:
    CitizenRepo();
    ~CitizenRepo() noexcept;

    size_t addCitizen(const char* name, ProffType type, 
                    int happ, int money, int life,
                    size_t lastCitOff, unsigned int id, 
                    BuildingType bType, int currDay);
    
    bool readCitizenAt(size_t offset, CitizenPack& out);
    bool removeCitizen(size_t offset, int currDay);
    
    bool loadCitizens(std::ifstream& in);
    bool saveCitizens(std::ofstream& out);

    class Iterator
    {
    public:
        Iterator(std::fstream& stream, int day, bool flag = false);
        
        const CitizenPack& operator*() { return currPack; }

        Iterator& operator++();
        bool operator!=(const Iterator& oth) const;
        bool operator==(const Iterator& oth) const;
    
    private:
        void readNextAlive();    

    private:
        std::fstream& file;
        CitizenPack currPack;
        bool end;
        int currDay;
    };

    Iterator begin(int currDay);
    Iterator end();
private:
    int calculateNaturalRemoveDay(int life, int currDay);

    static constexpr const char* CITIZEN_FILENAME = "..\\files\\citizen.bin";
    static constexpr const char* NAMES_FILENAME = "..\\files\\names.bin";

private:
    std::fstream citizens;
    std::fstream names;
};

#endif //CITIZEN_REPO_H
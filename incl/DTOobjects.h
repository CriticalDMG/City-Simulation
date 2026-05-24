#include <iostream>

#pragma pack(push, 1)
struct CitizenDTO
{
    int startHapp :7;
    int startLife :7;
    int proff     :4;
    int nameLen   :14;
    int startMoney;

    int creationDay;
    int deathDay;

    size_t nameOffset;
};

struct BuildingDTO
{
    int row;
    int col;

    int type            :4;
    int maxCitCount     :28;

    int currPplCounts;

    size_t FirstResOffset;
};
#pragma pack(pop)
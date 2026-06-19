#include "..\\incl\\Time.h"
#include "..\\incl\\CitizenRepo.h"
#include "..\\incl\\Register.h"
#include "..\\Logger\\logger.h"

int main()
{
    ProffRegister reg;
    CitizenRepo repo;
    CitizenPack pack;
    uint64_t offset = 0;
    offset = repo.addCitizen("Pesho", reg.GetProff((ProffType)1), 10, 10, 10, 0, 1, (BuildingType)1, 1, 1500);
    repo.readCitizenAt(offset, pack);

    std::cout << pack.salary << " " << pack.startHapp;
    return EXIT_SUCCESS;
}

#include "..\\incl\\Time.h"
#include "..\\incl\\building.h"

int main()
{
    Modern* mod = new Modern();
    uniquePointer<Building> b(new Building(49, 49, mod, 50));

    std::cout << b;
    return 0;
}


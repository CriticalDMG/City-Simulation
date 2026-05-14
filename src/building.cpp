#include "..\incl\building.h"

Building::Building(int n, int m, BuildType* type, size_t max)
:loc(n, m), type(type), rent(type->calcRent(loc, 100, 100)), MaxCitCount(max) {}
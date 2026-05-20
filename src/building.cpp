#include "..\incl\building.h"
#include "..\\Logger\\logger.h"


Building::Building(int n, int m, BuildType* type, unsigned int max)
:loc(n, m), type(type), rent(type->calcRent(loc, 100, 100)), MaxCitCount(max), ppl() { AUTO_LOG(); }

bool Building::addPerson(Citizen person)
{
    AUTO_LOG();
    if(ppl.size()>= MaxCitCount) return false;

    ppl.push_back(std::move(person));

    return true;
}

void Building::removePerson(const std::string& name)
{
    AUTO_LOG();
    for (auto it = ppl.begin(); it != ppl.end(); ++it)
    {
        if (it->GetName() == name) 
        {
            ppl.erase(it);
            break; 
        }
    }
}
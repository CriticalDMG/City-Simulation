#include "..\\incl\\citizen.h"
#include "..\\Logger\\logger.h"
#include "..\\incl\\helpers.h"
#include <cstring>

Citizen::Citizen(std::string name, const Proffesion* pr, 
                 int happiness, int life, unsigned int startMoney, 
                 int creationDay, unsigned int buildingId, BuildingType type)
: proff(pr), name(std::move(name)), happ(happiness),
  startHapp(happ), life(life), startLife(life), buildingId(buildingId),
  salary(helpers::calcSalary(pr->minSalary(), pr->maxSalary())),
  startMoney(startMoney), money(startMoney + salary), creationDay(creationDay)
{    
    AUTO_LOG();

    if(this->name.empty() || this->name.length() == 0)
    {
        LOG_ERROR(ERROR_CODES::INVALID_CHARACTERISTICS);
        throw (int)ERROR_CODES::INVALID_CHARACTERISTICS;
    }

    if(happiness < 0 || happiness > 100 || life < 0 || life > 100)
    {
        LOG_ERROR(ERROR_CODES::INVALID_CHARACTERISTICS);
        throw (int)ERROR_CODES::INVALID_CHARACTERISTICS;
    }

    if(type == DORM && proffesion() != STUDENT)
    {
        LOG_ERROR(ERROR_CODES::INVALID_DORM_PLACEMENT);
        throw (int)ERROR_CODES::INVALID_DORM_PLACEMENT;
    }
}

Citizen::Citizen(const CitizenPack& obj, 
                    std::string name, const Proffesion* proff, int rent)
: proff(proff), name(std::move(name)), happ(0),
  startHapp(obj.startHapp), life(0), startLife(obj.startLife), buildingId(obj.buildingId),
  salary(obj.salary), startMoney(startMoney), 
  money(startMoney + salary), creationDay(obj.creationDay)
{
    updateStatistics(Time::obj().GetDay(), rent);
}
                    
Citizen::operator CitizenPack() const
{
    CitizenPack pack;

    pack.startHapp = startHapp;
    pack.startLife = startLife;
    pack.proff = proffesion();
    pack.salary = salary;

    pack.nameLen = name.size();
    pack.startMoney = startMoney;

    pack.creationDay = creationDay;
    pack.remDay = -1;

    pack.buildingId = buildingId;
    
    pack.nameOffset = 0;
    pack.prevCitOffset = 0;

    return pack;
}

void Citizen::updateStatistics(int currDay, int toPay)
{
    AUTO_LOG();

    int lived = currDay - creationDay;

    if(lived <= 0)
    {
        money = startMoney;
        life = startLife;
        happ = startHapp;
        return;
    }

    int month = Time::obj().GetPassedMonths(creationDay, currDay);

    long long virtualMoney = startMoney + month * GetSalary() - (month * toPay) - (lived * FOOD);
    long long deficit = 0;

    if(virtualMoney <= 0)
    {
        money = 0;
        deficit = -virtualMoney;
    }
    else
    {
        money = virtualMoney;
    }

    int penalty = deficit / FOOD;

    int baseHappiness = proff->CalcHappiness(startHapp, month);
    int baseLife = proff->CalcLife(startLife, month);

    happ = std::max(0, baseHappiness - (penalty * 2));
    life = std::max(0, baseLife - (penalty * 1));

    if(happ == 0)
    {
        life = std::max(static_cast<int>(life) - 1, 0);
    }

    if(life <= 0)
    {
        happ = 0;
        money = 0;
    }
}


bool Citizen::operator==(const Citizen& oth) const
{
    return name == oth.name;
}

bool Citizen::operator!=(const Citizen& oth) const
{
    return !(*this == oth);
}

std::ostream& operator<<(std::ostream& os, const Citizen& s)
{
    AUTO_LOG();
    os << "\n";
    os << "Name: " << s.GetName() << "\n"
       << "Life: " << s.GetLife() << "\n"
       << "Happiness: " << s.happiness() << "\n"
       << "Proffesion: " << helpers::ToProffession(s.proffesion()) << "\n"
       << "Balance: " << s.balance() << "\n"; 
    return os;
}



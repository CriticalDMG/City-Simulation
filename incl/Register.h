#ifndef PROFFESION_REGISTER_H
#define PROFFESION_REGISTER_H

#include "proffesion.h"
#include "BuildType.h"
#include "building.h"

//uses the Flyweight/Registry design pattern to avoid instantiating 
//multiple identical objects for Professions and Building Types
//provides shared, memory-efficient pointers to global behaviors
class ProffRegister
{
public:
    ProffRegister() = default;

    const Proffesion* GetProff(ProffType type)
    {
        switch(type)
        {
            case TEACHER: return &teacher;
            case PROGRAMMER: return &prog;
            case MINER: return &miner;
            case STUDENT: return &student;
            case UNEMPLOYED: return &unemployed;
        }

        return nullptr;
    }

private:
    Teacher teacher;
    Programmer prog;
    Miner miner;
    Student student;
    Unemployed unemployed;
};

class BuildingRegister 
{
public:
    const BuildType* GetType(BuildingType type) const
    {
        switch(type) 
        {
            case MODERN: return &modern;
            case PANEL: return &panel;
            case DORM: return &dorm;
        }

        return nullptr;
    }

private:
    Modern modern;
    Panel panel;
    Dorm dorm;
};

#endif //PROFFESION_REGISTER_H
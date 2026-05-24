#include "..\\incl\\factory.h"

Building SimFactory::createBuilding(int row, int col, BuildingType type, unsigned int MaxCit)
{
    BuildType* building = nullptr;

    switch(type)
    {
        case MODERN: building = new Modern(); break;
        case PANEL: building = new Panel(); break;
        case DORM: building = new Dorm(); break;
        default:
            throw std::invalid_argument("Unknown building type!");
    }

    return Building(row, col, building, MaxCit);
}

Citizen SimFactory::createCitizen(std::string name, ProffType proffType, 
                                 unsigned int salary, int happiness, int life, 
                                 unsigned int startMoney, int creationDay, 
                                 BuildingType bType)
{
    Proffesion* proff = nullptr;
    
    switch(proffType)
    {
        case TEACHER: proff = new Teacher(salary); break;
        case MINER: proff = new Miner(salary); break;
        case PROGRAMMER: proff = new Programmer(salary); break;
        case STUDENT: proff = new Student(salary); break;
        case UNEMPLOYED: proff = new Unemployed(salary); break;
        default:
            throw std::invalid_argument("Unknown profession type!");
    }

    return Citizen(std::move(name), proff, happiness, life, startMoney, creationDay, bType);
}
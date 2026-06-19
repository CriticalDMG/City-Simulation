#ifndef PROFFESTION_H
#define PROFFESTION_H
#include <iostream>
#include "types.h"

class Proffesion
{
public:
    virtual int CalcHappiness(int happ, int monthsAdv) const { return happ; }
    virtual int CalcLife(int life, int monthsAdv) const { return life; }

    virtual int minSalary() const = 0;
    virtual int maxSalary() const = 0;


    virtual ProffType GetProff() const = 0;
protected:
};

class Teacher: public Proffesion
{
public:
    virtual int CalcHappiness(int happ, int monthsAdv) const override;

    virtual ProffType GetProff() const override { return TEACHER; }

    virtual int minSalary() const override { return 1200; }
    virtual int maxSalary() const override { return 1300; }
};

class Programmer: public Proffesion
{
public:
    virtual int CalcHappiness(int happ, int monthsAdv) const override;

    virtual ProffType GetProff() const override { return PROGRAMMER; }

    virtual int minSalary() const override { return 2000; }
    virtual int maxSalary() const override { return 5000; }
};

class Miner: public Proffesion
{
public:
    virtual int CalcLife(int life, int monthsAdv) const override;    

    virtual ProffType GetProff() const override { return MINER; }

    virtual int minSalary() const override { return 1000; }
    virtual int maxSalary() const override { return 3000; }
};

class Student: public Proffesion
{
public:
    virtual int CalcHappiness(int happ, int monthsAdv) const override;

    virtual ProffType GetProff() const override { return STUDENT; }

    virtual int minSalary() const override { return 0; }
    virtual int maxSalary() const override { return 0; }
};

class Unemployed: public Proffesion
{
public:
    virtual int CalcLife(int life, int monthsAdv) const override;    
    virtual int CalcHappiness(int happ, int monthsAdv) const override;

    virtual ProffType GetProff() const override { return UNEMPLOYED; }

    
    virtual int minSalary() const override { return 0; }
    virtual int maxSalary() const override { return 0; }
};

#endif //PROFFESTION_H
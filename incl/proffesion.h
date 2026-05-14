#ifndef PROFFESTION_H
#define PROFFESTION_H
#include <iostream>

class Proffesion
{
public:
    Proffesion(size_t sal): wage(sal) {}

    virtual void CalcHappiness(int& happ) const {}
    virtual void CalcLife(int& life) const {}

    virtual const char* GetProff() const = 0;

    size_t salary() const { return wage; }
protected:
    static unsigned int chooseSalary(int min, int max);

private:
    size_t wage;
};

class Teacher: public Proffesion
{
public:
    Teacher():Proffesion(Proffesion::chooseSalary(1200, 1300)) {}

    virtual void CalcHappiness(int& happ) const override;

    virtual const char* GetProff() const override { return "Teacher"; }
};

class Programmer: public Proffesion
{
public:
    Programmer(): Proffesion(Proffesion::chooseSalary(2000, 5000)) {}

    virtual void CalcHappiness(int& happ) const override;

    virtual const char* GetProff() const override { return "Programmer"; }
};

class Miner: public Proffesion
{
    Miner():Proffesion(Proffesion::chooseSalary(1000, 3000)) {}

    virtual void CalcLife(int& life) const override;    

    virtual const char* GetProff() const override { return "Miner"; }
};

class Student: public Proffesion
{
public:
    Student():Proffesion(0) {}

    virtual void CalcHappiness(int& happ) const override;

    virtual const char* GetProff() const override { return "Student"; }
};

class Jobless: public Proffesion
{
public:
    Jobless():Proffesion(0) {}

    virtual void CalcLife(int& life) const override;    
    virtual void CalcHappiness(int& happ) const override;
    
    virtual const char* GetProff() const override { return "Jobless"; }
};

#endif //PROFFESTION_H
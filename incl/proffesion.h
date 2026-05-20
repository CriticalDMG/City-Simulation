#ifndef PROFFESTION_H
#define PROFFESTION_H
#include <iostream>

enum ProffType
{
    UNKNOWN = -1,
    TEACHER,
    PROGRAMMER,
    MINER,
    STUDENT,
    UNEMPLOYED
};

class Proffesion
{
public:
    Proffesion(unsigned int sal): wage(sal) {}

    virtual void CalcHappiness(int& happ) const {}
    virtual void CalcLife(int& life) const {}

    virtual ProffType GetProff() const = 0;

    unsigned int salary() const { return wage; }
protected:

private:
    unsigned int wage;
};

class Teacher: public Proffesion
{
public:
    Teacher(unsigned int sal):Proffesion(sal) 
    {
        if(sal < 1200 || sal > 1300) 
            throw std::invalid_argument("Teacher salary should be betwenn 1200 and 1300");
    }

    virtual void CalcHappiness(int& happ) const override;

    virtual ProffType GetProff() const override { return TEACHER; }
};

class Programmer: public Proffesion
{
public:
    Programmer(unsigned int sal): Proffesion(sal) 
    {
        if(sal < 2000 || sal > 5000) 
            throw std::invalid_argument("Teacher salary should be betwenn 2000 and 5000");
    }

    virtual void CalcHappiness(int& happ) const override;

    virtual ProffType GetProff() const override { return PROGRAMMER; }
};

class Miner: public Proffesion
{
    Miner(unsigned int sal):Proffesion(sal) 
    {
        if(sal < 1000 || sal > 3000) 
            throw std::invalid_argument("Teacher salary should be betwenn 1000 and 3000");
    }

    virtual void CalcLife(int& life) const override;    

    virtual ProffType GetProff() const override { return MINER; }
};

class Student: public Proffesion
{
public:
    Student(unsigned int sal):Proffesion(sal) 
    {
        if(sal != 0) throw std::invalid_argument("Student salary should be 0");
    }

    virtual void CalcHappiness(int& happ) const override;

    virtual ProffType GetProff() const override { return STUDENT; }
};

class Unemployed: public Proffesion
{
public:
    Unemployed(unsigned int sal):Proffesion(sal) 
    {
        throw std::invalid_argument("Unemployed salary should be 0");
    }

    virtual void CalcLife(int& life) const override;    
    virtual void CalcHappiness(int& happ) const override;
    
    virtual ProffType GetProff() const override { return UNEMPLOYED; }
};

#endif //PROFFESTION_H
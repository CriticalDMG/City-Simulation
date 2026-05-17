#ifndef HISTORY_H
#define HISTORY_H
#include "Time.h"
#include "..\\Logger\\logger.h"
#include <fstream>

enum Change
{
    MONEY,
    LIFE,
    HAPPINESS
};


class History
{
public:
    History(std::string filename);

    bool write(Time& t, Change change, int amount);
    void print() const;

    bool GlobalSave(std::ofstream& GlobalSaveFile) const;
private:
    std::string filename;
};
#endif //HISTORY_H
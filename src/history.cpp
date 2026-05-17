#include "..\incl\history.h"
#include <cstring>

History::History(std::string filename)
:filename(std::move(filename))
{
    AUTO_LOG();

    std::ofstream of(filename, std::ios::binary | std::ios::app);
    if(!of.is_open()) throw "Culdnt create file!";
    of.close();
}

bool History::write(Time& t, Change change, int amount)
{
    AUTO_LOG();
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if(!file.is_open())
    {
        return false;
    }


    const char* time = t.GetTime();

    file.write(time, strlen(time));
    file.write(reinterpret_cast<const char*>(&change), sizeof(change));
    file.write(reinterpret_cast<const char*>(&amount), sizeof(amount));

    if(!file.good()) return false;

    file.close();

    return true;
}

void History::print() const
{
    AUTO_LOG();
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open() || !file.good())
    {
        return;
    }

    int amount;
    char buffer[25];
    Change change;

    while(!file.eof() && file.good())
    {
        file.read(buffer, sizeof(buffer));
        file.read(reinterpret_cast<char*>(&change), sizeof(change));
        file.read(reinterpret_cast<char*>(&amount), sizeof(amount));
    }

    if(!file.eof() || !file.good())
    {

    }

    file.close();
}

bool History::GlobalSave(std::ofstream& GlobalSaveFile) const
{
    AUTO_LOG();
    std::ifstream file(filename);
    if(!GlobalSaveFile.good() || !file.is_open()) return false;

    int amount;
    char buffer[25];
    Change change;

    while(!file.eof() && file.good())
    {
        file.read(buffer, sizeof(buffer));
        file.read(reinterpret_cast<char*>(&change), sizeof(change));
        file.read(reinterpret_cast<char*>(&amount), sizeof(amount));

        GlobalSaveFile.write(buffer, strlen(buffer));
        GlobalSaveFile.write(reinterpret_cast<const char*>(&change), sizeof(change));
        GlobalSaveFile.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
    }

    if(!file.eof() || !file.good() || !GlobalSaveFile.good())
    {
        return false;
    }

    return true;
}

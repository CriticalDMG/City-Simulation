#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>
#include "FileEngine.h"
#include "Register.h"
#include "command.h"

//acts as the 'Context' in the Command Design Pattern
//owns the FileEngine and manages the primary CLI event loop
class Simulation
{
public:
    Simulation();
    ~Simulation();

    void run();

    FileEngine* getEngine() { return engine; }
    void setEngine(FileEngine* newEngine); 
    
    bool hasUnsaved() const { return hasUnsavedChanges; }
    void setUnsaved(bool state) { hasUnsavedChanges = state; }
    
    void stop() { isRunning = false; } 

    ProffRegister& getPReg() { return pReg; }
    BuildingRegister& getBReg() { return bReg; }

private:
    std::vector<std::string> parseLine(const std::string& line);
    void registerCommands(); 

private:
    FileEngine* engine;
    bool hasUnsavedChanges;
    bool isRunning;

    ProffRegister pReg;
    BuildingRegister bReg;

    struct CommandEntry 
    {
        std::string name;
        Command* cmd;
    };

    std::vector<CommandEntry> commands; 
};

#endif // SIMULATION_H
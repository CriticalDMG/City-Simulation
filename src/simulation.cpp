#include "..\\incl\\simulation.h"
#include "..\\incl\\command.h"
#include <iostream>

Simulation::Simulation()
:engine(nullptr), hasUnsavedChanges(false), isRunning(true) 
{
    registerCommands();
}

Simulation::~Simulation()
{
    if(engine) delete engine;
    
    for(size_t i = 0; i < commands.size(); ++i) 
    {
        delete commands[i].cmd;
    }
}

void Simulation::setEngine(FileEngine* newEngine)
{
    if(engine) delete engine;
    engine = newEngine;
}

void Simulation::registerCommands()
{
    commands.push_back({"add", new AddCommand()});
    commands.push_back({"step", new StepCommand()});
    commands.push_back({"exit", new ExitCommand()});
    commands.push_back({"generate", new GenerateCommand()});
    commands.push_back({"remove", new RemoveCommand()});
    commands.push_back({"info", new InfoCommand()});
    commands.push_back({"stat", new StatCommand()});
    commands.push_back({"save", new SaveCommand()});
    commands.push_back({"load", new LoadCommand()});
}

std::vector<std::string> Simulation::parseLine(const std::string& line)
{
    std::vector<std::string> tokens;
    std::string current = "";
    
    for(char c : line)
    {
        if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
        {
            if(!current.empty())
            {
                tokens.push_back(current);
                current = "";
            }
        }
        else
        {
            current += c;
        }
    }
    if(!current.empty())
    {
        tokens.push_back(current);
    }
    
    return tokens;
}

void Simulation::run()
{
    std::cout << "  City Simulation  \n";
    std::string line;
    std::cout << "<";
    while(isRunning && std::getline(std::cin, line))
    {
        std::vector<std::string> args = parseLine(line);
        if(args.empty()) 
        { 
            std::cout << "> no commands entered!\n>"; 
            continue; 
        }

        std::string cmdName = args[0];
        bool commandFound = false;

        try 
        {
            for(size_t i = 0; i < commands.size(); ++i)
            {
                if(commands[i].name == cmdName)
                {
                    commands[i].cmd->execute(*this, args);
                    commandFound = true;
                    break;
                }
            }

            if(!commandFound) 
            {
                std::cout << "Unknown command!\n";
            }
        } 
        catch(...) 
        {
            std::cout << "Invalid arguments provided to command.\n";
        }

        if(isRunning)
        {
            std::cout << "> ";
        } 
    }
}
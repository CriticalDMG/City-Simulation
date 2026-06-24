#include "..\\incl\\simulation.h"
#include "..\\incl\\Paginator.h"
#include "..\\incl\\Time.h"
#include <iostream>
#include <cstdlib>

void AddCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    FileEngine* engine = context.getEngine();
    if(!engine) 
    { 
        std::cout << "No city loaded.\n"; 
        return; 
    }

    if(args.size() < 8) 
    { 
        std::cout << "Usage: add <n> <m> <name> <job> <happ> <money> <life>\n"; 
        return; 
    }

    int r = std::stoi(args[1]);
    int c = std::stoi(args[2]);

    std::string name = args[3];
    std::string job = args[4];

    ProffType pType = UNEMPLOYED;
    if(job == "Teacher") pType = TEACHER;
    else if(job == "Programmer") pType = PROGRAMMER;
    else if(job == "Miner") pType = MINER;
    else if(job == "Student") pType = STUDENT;

    if(engine->addCitizenTo(r, c, name.c_str(), context.getPReg().GetProff(pType), 
                                std::stoi(args[5]), std::stoi(args[6]), std::stoi(args[7])))
    {
        std::cout << "Citizen added!\n";
        context.setUnsaved(true);
    }
    else
    { 
        std::cout << "Failed to add citizen. Please check error_log.txt for details.\n"; 
    }
}

void StepCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    FileEngine* engine = context.getEngine();
    if(!engine) 
    { 
        std::cout << "No city loaded.\n"; 
        return; 
    }

    int days = (args.size() >= 2) ? std::stoi(args[1]) : 1;
    StepStats stats = engine->advanceTime(days);
    
    context.setUnsaved(true);
    std::cout << "Advanced " << days << " days.\n";
    std::cout << "Citizens with 0 happiness: " << stats.emptyHapp << "\n";
    std::cout << "Citizens with 0 money: " << stats.broke << "\n";
    std::cout << "Citizens with 0 life(removed): " << stats.removed << "\n";
}

void ExitCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    if(context.hasUnsaved())
    {
        std::cout << "Warning: Unsaved changes! Are you sure you want to exit?(y/n): ";
        std::string answer;
        std::getline(std::cin, answer);
        if (answer != "y" && answer != "Y") return; //stopping exit
    }
    std::cout << "Was fun having u around! Sad to see u go :(\n";
    context.stop(); //end of inf loop
}

void GenerateCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    if(args.size() < 3)
    {
        std::cout << "Usage: generate <n> <m>\n";
        return;
    }

    if(context.hasUnsaved())
    {
        std::cout << "You have unsaved changes. Type 'save <name>' first or 'exit' to discard.\n";
        return;
    }

    context.setEngine(nullptr);

    int n = std::stoi(args[1]);
    int m = std::stoi(args[2]);

    if(n <= 0 || m <= 0) 
    {
        std::cout << "Dimensions must be positive.\n";
        return;
    }

    FileEngine* newEngine = new FileEngine(n, m);
    context.setEngine(newEngine);
    
    const char* firstNames[] = {"Gencho", "Ivan", "Georgi", "Maria", "Elena", "Aleksandar", "Viktoria", "Dimitar", "Sofia", "Martin"};
    const char* lastNames[] = {"Ivanova", "Petrov", "Georgiev", "Dimitrova", "Popov", "Koleva", "Hristov", "Nikolova", "Todorov", "Iliev"};
    
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < m; j++)
        {
            //20% chance for a cell to be empty
            if(rand() % 100 < 20) continue;

            BuildingType bType = (BuildingType)(rand() % 3);
            
            //capacity depending on the building
            int maxCap = 0;
            if(bType == MODERN) maxCap = 10 + rand() % 1391; // 10 to 1300
            else if(bType == PANEL) maxCap = 50 + rand() % 1451; // 50 to 1500
            else if(bType == DORM) maxCap = 100 + rand() % 1401; // 100 to 2500

            newEngine->addBuildingTo(i, j, context.getBReg().GetType(bType), maxCap);
            
            //adding people(around 25% of a buildings capacity)
            int peopleCount = rand() % (maxCap / 4 + 1); 

            for(int p = 0; p < peopleCount; p++)
            {
                std::string fullName = std::string(firstNames[rand() % 10]) + " " + std::string(lastNames[rand() % 10]);
                
                ProffType pType;
                
                if(bType == DORM) 
                {
                    pType = STUDENT; 
                } 
                else 
                {
                    pType = (ProffType)(rand() % 5);
                }

                int happ = 50 + rand() % 51; // 50 to 100
                int life = 50 + rand() % 51; // 50 to 100
                int money = rand() % 5000; // 0 to 4999 начални пари

                if(peopleCount < maxCap) 
                {
                    newEngine->addCitizenTo(i, j, fullName.c_str(), context.getPReg().GetProff(pType), happ, money, life);
                }
            }
        }
    }
    
    context.setUnsaved(true);
    std::cout << "City generated successfully with dynamic population!\n";
    std::cout << "Simulation Start Date: " << Time::obj().getStartTime() << "\n";
}
void RemoveCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    FileEngine* engine = context.getEngine();
    if(!engine) 
    { 
        std::cout << "No city loaded.\n"; 
        return; 
    }

    if(args.size() < 4)
    {
        std::cout << "Usage: remove <n> <m> <name>\n";
        return;
    }

    int r = std::stoi(args[1]);
    int c = std::stoi(args[2]);
    std::string name = args[3];

    if(engine->removeCitizenFrom(r, c, name.c_str()))
    {
        std::cout << "Citizen removed successfully.\n";
        context.setUnsaved(true);
    }
    else
    {
        std::cout << "Failed to remove citizen. Please check error_log.txt for details.\n";
    }
}


void InfoCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    FileEngine* engine = context.getEngine();
    if(!engine) 
    { 
        std::cout << "No city loaded.\n"; 
        return; 
    }

    if(args.size() == 1)
    {
        Paginator::paginate(engine->getGlobalInfo(), 10);
    }
    else if(args.size() == 3)
    {
        int r = std::stoi(args[1]);
        int c = std::stoi(args[2]);
        Paginator::paginate(engine->getBuildingInfo(r, c), 10);
    }
    else if(args.size() >= 4)
    {
        int r = std::stoi(args[1]);
        int c = std::stoi(args[2]);
        std::string name = args[3];
        Paginator::paginate(engine->getCitizenInfo(r, c, name.c_str()), 10);
    }
    else
    {
        std::cout << "Usage: info | info <x> <y> | info <x> <y> <name>\n";
    }
}

void StatCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    FileEngine* engine = context.getEngine();
    if(!engine)
    { 
        std::cout << "No city loaded.\n"; 
        return; 
    }

    if(args.size() < 2)
    {
        std::cout << "Usage: stat <option> (happiness, money, life, profession, buildings)\n";
        return;
    }

    std::string option = args[1];

    auto cStats = engine->getCitizenStats();
    auto bStats = engine->getBuildingStats();

    if(cStats.aliveCit == 0 && option != "buildings")
    {
        std::cout << "No alive citizens to calculate stats.\n";
        return;
    }

    if(option == "happiness")
    {
        std::cout << "Happiness -> Avg: " << (cStats.happ / cStats.aliveCit) 
                  << " | Min: " << cStats.min[0] << " | Max: " << cStats.max[0] << "\n";
    }
    else if(option == "money")
    {
        std::cout << "Money -> Avg: " << (cStats.money / cStats.aliveCit) 
                  << " | Min: " << cStats.min[1] << " | Max: " << cStats.max[1] << "\n";
    }
    else if(option == "life")
    {
        std::cout << "Life -> Avg: " << (cStats.life / cStats.aliveCit) 
                  << " | Min: " << cStats.min[2] << " | Max: " << cStats.max[2] << "\n";
    }
    else if(option == "profession")
    {
        std::cout << "Professions:\n"
                  << "Teachers: " << cStats.proffStats[TEACHER] << "\n"
                  << "Programmers: " << cStats.proffStats[PROGRAMMER] << "\n"
                  << "Miners: " << cStats.proffStats[MINER] << "\n"
                  << "Students: " << cStats.proffStats[STUDENT] << "\n"
                  << "Unemployed: " << cStats.proffStats[UNEMPLOYED] << "\n";
    }
    else if(option == "buildings")
    {
        std::cout << "Building Distribution:\n"
                  << "Modern: " << bStats.type[MODERN] << "\n"
                  << "Panel: " << bStats.type[PANEL] << "\n"
                  << "Dorm: " << bStats.type[DORM] << "\n";
    }
    else
    {
        std::cout << "Unknown stat option.\n";
    }
}

void SaveCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    FileEngine* engine = context.getEngine();
    if(!engine) 
    { 
        std::cout << "No city loaded.\n"; 
        return; 
    }

    if(args.size() < 2)
    {
        std::cout << "Usage: save <name>\n";
        return;
    }

    std::string filename = args[1];

    if(engine->save(filename.c_str()))
    {
        std::cout << "Simulation saved to " << filename << "\n";
        context.setUnsaved(false);
    }
    else
    {
        std::cout << "Failed to save file. Please check error_log.txt for details.\n";
    }
}

void LoadCommand::execute(Simulation& context, const std::vector<std::string>& args)
{
    if(args.size() < 2)
    {
        std::cout << "Usage: load <name>\n";
        return;
    }

    if(context.hasUnsaved())
    {
        std::cout << "You have unsaved changes. Type 'save <name>' first or 'exit' to discard.\n";
        return;
    }

    std::string filename = args[1];
    
    context.setEngine(nullptr);

    try
    {
        FileEngine* newEngine = new FileEngine(filename.c_str());
        context.setEngine(newEngine);
        std::cout << "Simulation loaded from " << filename << "\n";
        context.setUnsaved(false);
    }catch(...)
    {
        std::cout << "Failed to load simulation. Please check error_log.txt for details.\n\n";
    }
}
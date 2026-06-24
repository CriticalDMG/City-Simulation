#include "..\\incl\\FileEngine.h"
#include "..\\incl\\Time.h"
#include <cstring>

FileEngine::FileEngine(int r, int c)
:currentDay(0), matrixRows(r), matrixCols(c), cStats(), bStats()
{}

FileEngine::FileEngine(const char* loadFilename)
:currentDay(0), matrixRows(0), matrixCols(0), cStats(), bStats()
{
    if(!load(loadFilename))
    {
        LOG_ERROR(ERROR_CODES::SAVE_FILE_CORRUPTION);
        throw (int)ERROR_CODES::SAVE_FILE_CORRUPTION;
    }
}

//serialization
bool FileEngine::save(const char* filename)
{
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) return false;

   std::time_t rawStart = Time::obj().getRawStartTime();
    
    out.write(reinterpret_cast<const char*>(&matrixRows), sizeof(matrixRows));
    out.write(reinterpret_cast<const char*>(&matrixCols), sizeof(matrixCols));
    out.write(reinterpret_cast<const char*>(&currentDay), sizeof(currentDay));
    
    out.write(reinterpret_cast<const char*>(&rawStart), sizeof(rawStart));

    buildingRepo.resetReadSequence();
    BuildingPack bPack;

    while(buildingRepo.save(out, bPack))
    {
        uint64_t countFileOffset = (uint64_t)out.tellp() - sizeof(int);
        
        int actualSavedCitizens = citizenRepo.saveCitizens(out, bPack.LastResOffset);

        uint64_t endPos = out.tellp();
        out.seekp(countFileOffset, std::ios::beg);
        out.write(reinterpret_cast<const char*>(&actualSavedCitizens), sizeof(actualSavedCitizens));
        out.seekp(endPos, std::ios::beg);
    }

    return true;
}

//deseralization
bool FileEngine::load(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open()) return false;

    in.read(reinterpret_cast<char*>(&matrixRows), sizeof(matrixRows));
    in.read(reinterpret_cast<char*>(&matrixCols), sizeof(matrixCols));
    in.read(reinterpret_cast<char*>(&currentDay), sizeof(currentDay));

    std::time_t rawStart;
    in.read(reinterpret_cast<char*>(&rawStart), sizeof(rawStart));
    Time::obj().syncWithSaveFile(rawStart, currentDay);
    
    info.clear(); 
    bStats = BuildingStats();

    BuildingPack bPack;
    int totalPeople = 0;

    // Accumulate building statistics natively during the reading sequence
    while(buildingRepo.readFromSave(in, bPack, totalPeople))
    {
        if(bPack.type >= 0 && bPack.type < BuildingType::TYPE_COUNTER) 
        {
            bStats.type[bPack.type]++;
        }

        uint64_t newOffset = buildingRepo.encodeBuilding(bPack);
        Identificator ident;
        ident.id = bPack.id;
        ident.fileOffset = newOffset;
        ident.capacity = bPack.maxCitCount;
        ident.currLiving = 0; //changed when citizens are loaded

        //stream the citizens directly into the binary file mapping
        BuildingPack updatedPack = citizenRepo.loadCitizens(in, bPack, totalPeople);
        
        buildingRepo.saveChanges(updatedPack, ident);
        info.push_back(ident);
    }

    advanceTime(0);

    return true;
}

bool FileEngine::addBuildingTo(int row, int col, const BuildType* type, unsigned int maxCapacity)
{
    if(!type) return false;

    uint64_t targetId = (uint64_t)row * matrixCols + col;

    Identificator ident = buildingRepo.addBuilding(targetId, type, row, col, matrixRows, matrixCols, maxCapacity);

    if(ident.fileOffset == 0) 
    {
        return false; 
    }

    BuildingType bType = type->GetType();
    if(bType >= 0 && bType < BuildingType::TYPE_COUNTER) 
    {
        bStats.type[bType]++;
    }
    
    int left = getPos(ident.id);
    info.insert(info.begin() + left, ident);

    return true;
}

bool FileEngine::addCitizenTo(int row, int col, const char* name, const Proffesion* job, int happ, int money, int life)
{
    uint64_t targetId = (uint64_t)row * matrixCols + col;
    Identificator* ident = findByID(targetId);
    
    if(!ident) return false;
    
    BuildingPack bPack = buildingRepo.getBulding(*ident);
    uint64_t currOff = bPack.LastResOffset;

    while(currOff != 0)
    {
        CitizenPack cPack;
        citizenRepo.readCitizenAt(currOff, cPack);
        
        int actualRemDay = (cPack.remDay == -1) ? 2000000000 : cPack.remDay;//if the citizen is with inf life 2'000'000'000
    
        int overlapStart = std::max(cPack.creationDay, currentDay);
        
        if (overlapStart < actualRemDay)
        {
            char buff[512];
            citizenRepo.names.seekg(cPack.nameOffset, std::ios::beg);
            citizenRepo.names.read(buff, cPack.nameLen);
            buff[cPack.nameLen] = '\0';

            if (strcmp(buff, name) == 0)
            {
                LOG_ERROR(ERROR_CODES::DUPLICATE_CITIZEN_NAME);
                return false;
            }
        }
        currOff = cPack.prevCitOffset;
    }

    int futurePeak = getFuturePeakOccupancy(bPack);
    
    if(futurePeak >= ident->capacity)
    {
        std::cout << "\n[TEMPORAL PARADOX PREVENTED] Cannot add citizen!\n"
                  << "Adding someone today will cause the building to overflow (exceed capacity " 
                  << ident->capacity << ") on a future day.\n";
        
        LOG_ERROR(ERROR_CODES::BUILDING_CAPACITY_EXCEEDED);
        return false;
    }
    
    uint64_t newCitOffset = citizenRepo.addCitizen(name, job, happ, money, life, 
                                                   bPack.LastResOffset, ident->id, 
                                                   (BuildingType)bPack.type, currentDay, bPack.rent);
    
    if(newCitOffset == 0) return false;

    bPack.LastResOffset = newCitOffset;
    buildingRepo.saveChanges(bPack, *ident);
        
    ident->currLiving++; 
    return true;
}

bool FileEngine::removeCitizenFrom(int row, int col, const char* name)
{
    uint64_t targetId = (uint64_t)row * matrixCols + col;
    Identificator* ident = findByID(targetId);
    
    if(!ident) return false; 

    BuildingPack bPack = buildingRepo.getBulding(*ident);

    if(citizenRepo.removeCitizen(bPack.LastResOffset, name, currentDay) == 1)
    {
        ident->currLiving--;
        return true; 
    }

    return false;
}

StepStats FileEngine::advanceTime(int days)
{
    StepStats sStats{};
    int oldDay = currentDay;
    
    if(days != 0)
    {
        Time::obj().advance(days);
        currentDay += days;
    }

    //reset runtime counters for global statistic recalculation
    for (auto& ident : info) { ident.currLiving = 0; }
    cStats = CitizenStats();// Default construction resets all metrics

    //When time-traveling backward, evaluate stats relative to the historical day    
    int evaluationDay = std::min(oldDay, currentDay);
    auto it = citizenRepo.begin(evaluationDay);
    auto endIt = citizenRepo.end();

    while(it != endIt)
    {
        const CitizenPack& pack = *it;
        ++it;

        //citizen died exactly within the advanced time-frame
        if(days > 0 && pack.remDay > oldDay && pack.remDay <= currentDay)
        {
            sStats.removed++;
            
            Identificator* ident = findByID(pack.buildingId);
            int rent = 0;
            if(ident) 
            {
                BuildingPack bPack = buildingRepo.getBulding(*ident);
                rent = bPack.rent;
            }

            const Proffesion* pType = proffReg.GetProff((ProffType)pack.proff);
            
            Citizen tempCit(pack, "", pType, rent);
            
            tempCit.updateStatistics(pack.remDay, rent);
            
            //incrementing the counter if the citizen is wit 0 balance or happ
            if(tempCit.happiness() == 0) sStats.emptyHapp++;
            if(tempCit.balance() == 0) sStats.broke++;
            
            continue; //exclude dead citizens from current stats
        }

        if(pack.creationDay <= currentDay && (pack.remDay == -1 || pack.remDay > currentDay))
        {
            Identificator* ident = findByID(pack.buildingId);
            int rent = 0;
            if(ident) 
            {
                ident->currLiving++;
                
                BuildingPack bPack = buildingRepo.getBulding(*ident);
                BuildingType bType = (BuildingType)bPack.type;

                rent = bPack.rent; 
            }

            const Proffesion* pType = proffReg.GetProff((ProffType)pack.proff);

            Citizen tempCit(pack, "", pType, rent);

            int currHapp = tempCit.happiness(); 
            int currMoney = tempCit.balance();  
            int currLife = tempCit.GetLife();   

            cStats.aliveCit++;
        
            if(pack.proff >= 0 && pack.proff < ProffType::PROFFESION_COUNTER) 
            {
                cStats.proffStats[pack.proff]++;
            }

            cStats.happ += currHapp;
            cStats.money += currMoney;
            cStats.life += currLife;

            //Min / Max happiness
            if(currHapp < cStats.min[0]) cStats.min[0] = currHapp;
            if (currHapp > cStats.max[0]) cStats.max[0] = currHapp;

            //Min / Max money
            if(currMoney < cStats.min[1]) cStats.min[1] = currMoney;
            if(currMoney > cStats.max[1]) cStats.max[1] = currMoney;

            //Min / Max life
            if(currLife < cStats.min[2]) cStats.min[2] = currLife;
            if(currLife > cStats.max[2]) cStats.max[2] = currLife;
        }
    }

    return sStats;
}

Identificator* FileEngine::findByID(uint64_t id)
{
    int left = 0;
    int right = info.size() - 1;

    while(left <= right)
    {
        int mid = left + (right - left) / 2;

        if(info[mid].id == id)
        {
            return &info[mid]; 
        }
        
        if(info[mid].id < id)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return nullptr; //no such building
}

int FileEngine::getPos(uint64_t id)
{
    int left = 0;
    int right = info.size() - 1;

    while(left <= right)
    {
        int mid = left + (right - left) / 2;
        
        if(info[mid].id < id) 
        {
            left = mid + 1;
        } 
        else 
        {
            right = mid - 1;
        }
    }

    return left;
}

std::vector<std::string> FileEngine::getBuildingInfo(int row, int col)
{
    std::vector<std::string> res;
    uint64_t targetId = (uint64_t)row * matrixCols + col;
    Identificator* ident = findByID(targetId);
    
    if(!ident) 
    {
        res.push_back("Location " + std::to_string(row) + " " + std::to_string(col) + " is an empty field.");
        return res;
    }

    BuildingPack bPack = buildingRepo.getBulding(*ident);
    BuildingType bType = (BuildingType)bPack.type;

    res.push_back("Location " + std::to_string(row) + " " + std::to_string(col) + ":");
    res.push_back("  <Building Type>: " + std::string(helpers::ToBuildingType(bType)));
    res.push_back("  <Rent>: " + std::to_string(bPack.rent));
    res.push_back("  <Capacity>: " + std::to_string(ident->capacity));
    res.push_back("  <Free Spots>: " + std::to_string(ident->capacity - ident->currLiving));
    res.push_back("  <Residents>:");

    uint64_t currOffset = bPack.LastResOffset;
    while(currOffset != 0)
    {
        CitizenPack cPack;
        citizenRepo.readCitizenAt(currOffset, cPack);

        //showing only ppl alive at currDya
        if(cPack.creationDay <= currentDay && (cPack.remDay == -1 || currentDay < cPack.remDay))
        {
            char buff[512];
            citizenRepo.names.seekg(cPack.nameOffset, std::ios::beg);
            citizenRepo.names.read(buff, cPack.nameLen);
            buff[cPack.nameLen] = '\0';

            const Proffesion* pType = proffReg.GetProff((ProffType)cPack.proff);
            
            Citizen tempCit(cPack, buff, pType, bPack.rent);

            res.push_back("    <Name>: " + std::string(buff));
            res.push_back("      Profession: " + std::string(helpers::ToProffession((ProffType)cPack.proff)));
            res.push_back("      Happiness: " + std::to_string(tempCit.happiness()));
            res.push_back("      Money: " + std::to_string(tempCit.balance()));
            res.push_back("      Life: " + std::to_string(tempCit.GetLife()));
        }
        currOffset = cPack.prevCitOffset;
    }

    return res;
}

std::vector<std::string> FileEngine::getGlobalInfo()
{
    std::vector<std::string> res;
    
    if(info.empty())
    {
        res.push_back("The city is currently completely empty.");
        return res;
    }

    for(const auto& ident : info)
    {
        int row = ident.id / matrixCols;
        int col = ident.id % matrixCols;
        
        auto bInfo = getBuildingInfo(row, col);
        res.insert(res.end(), bInfo.begin(), bInfo.end());
    }
    
    res.push_back("");
    res.push_back("   Chronological City History   ");

    struct CityEvent 
    {
        int day;
        std::string action;
        std::string citizenName;
    };
    std::vector<CityEvent> events;

    citizenRepo.citizens.clear();
    citizenRepo.citizens.seekg(sizeof(FileSignature), std::ios::beg);

    CitizenPack cPack;
    while(citizenRepo.citizens.read(reinterpret_cast<char*>(&cPack), sizeof(cPack)))
    {
        if(cPack.signature != CITY_SIG) continue;

        char buff[512];
        citizenRepo.names.clear();
        citizenRepo.names.seekg(cPack.nameOffset, std::ios::beg);
        citizenRepo.names.read(buff, cPack.nameLen);
        buff[cPack.nameLen] = '\0';
        std::string nameStr(buff);

        if (cPack.creationDay <= currentDay)
        {
            events.push_back({cPack.creationDay, "ARRIVED: Moved into building ID " + std::to_string(cPack.buildingId), nameStr});
        }

        if (cPack.remDay != -1 && cPack.remDay <= currentDay)
        {
            events.push_back({cPack.remDay, "DEPARTED: Evicted or died from building ID " + std::to_string(cPack.buildingId), nameStr});
        }
    }

    for(size_t i = 1; i < events.size(); i++) 
    {
        CityEvent key = events[i];
        int j = static_cast<int>(i) - 1;
        while (j >= 0 && events[j].day > key.day) 
        {
            events[j + 1] = events[j];
            j--;
        }
        events[j + 1] = key;
    }

    if(events.empty())
    {
        res.push_back("No historical events recorded up to Day " + std::to_string(currentDay) + ".");
    }
    else
    {
        for (const auto& ev : events)
        {
            std::time_t t = Time::obj().getRawStartTime() + (ev.day * SINGLEDAY);
            std::string dateStr = std::ctime(&t);
            dateStr.pop_back(); 

            res.push_back("[Day " + std::to_string(ev.day) + " | " + dateStr + "] " + ev.citizenName + " -> " + ev.action);
        }
    }
    
    return res;
}

std::vector<std::string> FileEngine::getCitizenInfo(int row, int col, const char* name)
{
    std::vector<std::string> res;
    uint64_t targetId = (uint64_t)row * matrixCols + col;
    Identificator* ident = findByID(targetId);
    
    if(!ident)
    {
        res.push_back("Building not found at this location.");
        return res;
    }

    BuildingPack bPack = buildingRepo.getBulding(*ident);
    uint64_t currOffset = bPack.LastResOffset;
    bool found = false;
    CitizenPack tPack;

    while(currOffset != 0)
    {
        citizenRepo.readCitizenAt(currOffset, tPack);
        
        if(tPack.creationDay <= currentDay) 
        {
            char buff[512];
            citizenRepo.names.seekg(tPack.nameOffset, std::ios::beg);
            citizenRepo.names.read(buff, tPack.nameLen);
            buff[tPack.nameLen] = '\0';

            if(strcmp(buff, name) == 0) 
            {
                found = true;
                break;
            }
        }
        currOffset = tPack.prevCitOffset;
    }

    if(!found) 
    {
        res.push_back("Citizen " + std::string(name) + " never lived at this location.");
        return res;
    }

    const Proffesion* pType = proffReg.GetProff((ProffType)tPack.proff);
    Citizen tempCit(tPack, name, pType, bPack.rent);

    res.push_back("=== Full Information for Citizen: " + std::string(name) + " ===");
    res.push_back("  Profession: " + std::string(helpers::ToProffession((ProffType)tPack.proff)));
    res.push_back("  Salary: " + std::to_string(tPack.salary));
    res.push_back("  Born on Day: " + std::to_string(tPack.creationDay));
    
    if(tPack.remDay != -1 && tPack.remDay <= currentDay) 
    {
        res.push_back("  Status: DEAD (Died on Day " + std::to_string(tPack.remDay) + ")");
    } 
    else 
    {
        res.push_back("  Status: ALIVE");
    }

    res.push_back("\n   Life History (Changes in characteristics)   ");

    int prevHapp = -1, prevMoney = -1, prevLife = -1;
    
    //if remDay is -1, we do a loop to currDay
    //else to deathDay
    int endDay = (tPack.remDay == -1) ? currentDay : std::min(currentDay, tPack.remDay);
    
    for(int day = tPack.creationDay; day <= endDay; ++day)
    {
        tempCit.updateStatistics(day, bPack.rent);

        int currHapp = tempCit.happiness();
        int currMoney = tempCit.balance();
        int currLife = tempCit.GetLife();

        //if there is a change to whichever characteristic it is saved
        if (currHapp != prevHapp || currMoney != prevMoney || currLife != prevLife)
        {
            //generating the real date of the chage
            std::time_t t = Time::obj().getRawStartTime() + (day * SINGLEDAY);
            std::string dateStr = std::ctime(&t);
            dateStr.pop_back();

            res.push_back("Day " + std::to_string(day) + " (" + dateStr + "):");
            res.push_back("    Happiness: " + std::to_string(currHapp) + 
                          ", Money: " + std::to_string(currMoney) + 
                          ", Life: " + std::to_string(currLife));

            prevHapp = currHapp;
            prevMoney = currMoney;
            prevLife = currLife;
        }
    }

    return res;
}

int FileEngine::getFuturePeakOccupancy(const BuildingPack& bPack)
{
    int maxOccupancy = 0;
    int maxFutureDay = currentDay;
    
    uint64_t currOff = bPack.LastResOffset;
    std::vector<CitizenPack> residents; 
    
    while(currOff != 0) 
    {
        CitizenPack cPack;
        citizenRepo.readCitizenAt(currOff, cPack);
        residents.push_back(cPack);
        
        if (cPack.creationDay > maxFutureDay) 
        {
            maxFutureDay = cPack.creationDay;
        }
        
        currOff = cPack.prevCitOffset;
    }
    
    for (int d = currentDay; d <= maxFutureDay; ++d) 
    {
        int livingOnDayD = 0;
        
        for (const auto& c : residents) 
        {
            //citizen is considered alive on day 'd' if they were born on/before 'd' 
            //and haven't died (or are strictly immortal: -1)
            if (c.creationDay <= d && (c.remDay == -1 || c.remDay > d)) 
            {
                livingOnDayD++;
            }
        }
        
        //write of the absolute peak of living citizens
        if (livingOnDayD > maxOccupancy) 
        {
            maxOccupancy = livingOnDayD;
        }
    }
    
    return maxOccupancy;
}
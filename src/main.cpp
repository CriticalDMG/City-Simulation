#include "..\\incl\\Time.h"
#include "..\\incl\\building.h"
#include "..\\Logger\\logger.h"

int main()
{
    AUTO_LOG();
    Logger::GetInstance();
    Time& t = Time::obj();

    Modern* b = new Modern();
    Proffesion* proff = new Programmer(5000);
    
    Building build(4, 5, b, 50);
    build.addPerson(Citizen("Gerorgi", proff, 100, 100, 20000, t.GetDay(), build.GetType()));

    std::cout << t.GetCurrentTime() << "\n";
    std::cout << t.getStartTime() << "\n";
    std::cout << t.GetDay() << "\n";

    build[0].updateStatistics(t.GetDay(), build.GetRent());
    std::cout << build;
    
    return 0;
}

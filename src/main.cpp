#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

#include "..\\incl\\simulation.h"

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    Simulation app;
    app.run();

    return 0;
}
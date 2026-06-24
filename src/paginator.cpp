#include "..\\incl\\Paginator.h"
#include <iostream>

void Paginator::paginate(const std::vector<std::string>& lines, size_t linesPerPage)
{
    if(lines.empty()) return;

    size_t totalLines = lines.size();
    
    for(size_t i = 0; i < totalLines; ++i)
    {
        std::cout << lines[i] << "\n";

        if((i + 1) % linesPerPage == 0 && (i + 1) < totalLines)
        {
            std::cout << "\n--- Displayed lines: " << (i + 1) << " out of " << totalLines << " ---\n";
            std::cout << "[Press Enter for next page, or 'q' / 'exit' to stop]: ";
            
            std::string input;
            std::getline(std::cin, input);

            if (input == "q" || input == "Q" || input == "exit")
            {
                std::cout << "--- Output stopped by user. ---\n\n";
                return;
            }
                
        }
    }

    std::cout << "End of info.\n\n";
}
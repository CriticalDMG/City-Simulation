#ifndef PAGINATOR_H
#define PAGINATOR_H

#include <vector>
#include <string>

//utility class fulfilling the requirement to display long informational
//outputs in a fragmented, user-friendly paginated format
class Paginator
{
public:
    static void paginate(const std::vector<std::string>& lines, size_t linesPerPage = 10);
};

#endif // PAGINATOR_H
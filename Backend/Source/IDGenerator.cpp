#include "../Include/IDGenerator.h"

map <int, int> IDGenerator::nextIDMap;
int IDGenerator::lastYear = 0;

string IDGenerator::generateID(int prefix) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentYear = (now->tm_year + 1900) % 100;

    if (currentYear != lastYear) {
        nextIDMap.clear();
        lastYear = currentYear;
    }

    int& nextID = nextIDMap[prefix];

    ostringstream oss;
    oss << setw(2) << setfill('0') << prefix
        << setw(2) << setfill('0') << currentYear
        << setw(4) << setfill('0') << ++nextID;
    return oss.str();
}

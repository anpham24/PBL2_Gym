#include "../Include/IDGenerator.h"

int IDGenerator::nextID[7] = {0};
int IDGenerator::lastYear = 0;

string IDGenerator::generateID(int prefix) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentYear = (now->tm_year + 1900) % 100;

    if (currentYear != lastYear) {
        for (int i = 0; i < 10; ++i)
            nextID[i] = 0;
        lastYear = currentYear;
    }

    ostringstream oss;
    oss << setw(2) << setfill('0') << prefix
        << setw(2) << setfill('0') << currentYear
        << setw(4) << setfill('0') << ++nextID[prefix];
    return oss.str();
}

// ID có dạng: 10250001 (10: NV, 25: năm đk 2025, 0001: số tt 0001)
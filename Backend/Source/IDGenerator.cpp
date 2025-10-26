#include "../Include/IDGenerator.h"

int IDGenerator::nextID[8] = {0};
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

    string result;
    result += to_string(prefix) + '0';
    result += to_string(currentYear);
    string temp = to_string(++nextID[prefix]);
    while (temp.length() < 4)
        temp = '0' + temp;
    result += temp;
    return result;
}

// ID có dạng: 10250001 (10: NV, 25: năm đk 2025, 0001: số tt 0001)
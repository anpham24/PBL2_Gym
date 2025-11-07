#include "IDGenerator.h"
#include <fstream>

int IDGenerator::nextID[10] = {0};
int IDGenerator::lastYear = 0;

void IDGenerator::loadState() {
    ifstream fileIn("Data/IDGeneratorState.txt");
    
    if (fileIn.is_open()) {
        fileIn >> lastYear;
        for (int i = 1; i <= 9; ++i) {
            fileIn >> nextID[i];
        }
        fileIn.close();
    } else {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        lastYear = 1900 + ltm->tm_year;
        
        for (int i = 0; i < 10; ++i) {
            nextID[i] = 0; 
        }
        saveState();
    }
}

void IDGenerator::saveState() {
    ofstream fileOut("Data/IDGeneratorState.txt");
    
    if (fileOut.is_open()) {
        fileOut << lastYear << endl;
        for (int i = 1; i <= 9; ++i) {
            fileOut << nextID[i] << endl;
        }
        fileOut.close();
    }
}

string IDGenerator::generateID(int prefix) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int currentYear = (now->tm_year + 1900) % 100;

    if (currentYear != lastYear) {
        for (int i = 1; i <= 9; ++i)
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
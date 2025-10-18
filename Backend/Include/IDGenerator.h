#pragma once
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class IDGenerator {
    private:
        static map<int, int> nextIDMap;
        static int lastYear;
    public:
        static string generateID(int prefix);
};
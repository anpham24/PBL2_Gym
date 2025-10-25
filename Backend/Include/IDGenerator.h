#pragma once
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
using namespace std;

class IDGenerator {
    private:
        static int nextID[7];
        static int lastYear;
    public:
        static string generateID(int prefix);
};
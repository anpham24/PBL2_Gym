#pragma once
#include <string>
#include <ctime>
using namespace std;

class IDGenerator {
    private:
        static int nextID[8];
        static int lastYear;
    public:
        static string generateID(int prefix);
};
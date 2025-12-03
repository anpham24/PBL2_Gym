#pragma once
#include <string>

#include "MyHashTable.h"


using namespace std;

class LogTapPT;

class LogTapPTService {
public:
    static void themLogTapPT(const string& maHV, const string& maHLV, const string& ngayGio);
    static void suaLogTapPT(const string& maLog, const string& maHV, const string& maHLV, 
                           const string& ngayGio);
    static const MyHashTable<LogTapPT*>& getAllLogTapPT();
};

#pragma once
#include <string>
using namespace std;

class LogTapPTService {
public:
    static void themLogTapPT(const string& maHV, const string& maHLV, const string& ngayGio);
    static void suaLogTapPT(const string& maLog, const string& maHV, const string& maHLV, 
                           const string& ngayGio);
};

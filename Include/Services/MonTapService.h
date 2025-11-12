#pragma once
#include <string>
using namespace std;

class MonTapService {
public:
    static void themMonTap(const string& tenMon);
    static void suaMonTap(const string& maMon, const string& tenMon);
    static void xoaMonTap(const string& maMon);
};

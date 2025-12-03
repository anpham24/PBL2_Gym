#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class MonTap;

class MonTapService {
public:
    static void themMonTap(const string& tenMon);
    static void suaMonTap(const string& maMon, const string& tenMon);
    static void xoaMonTap(const string& maMon);
    static const MyVector<MonTap*>& getAllMonTap();
};

#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class GoiTap;

class GoiTapService {
public:
    static void themGoiTap(const string& tenGoi, int thoiGian, int soBuoiPT, double gia);
    static void themMonTapVaoGoiTap(const string& maGoi, const string& maMon);
    static void suaGoiTap(const string& maGoi, const string& tenGoi, int thoiGian, int soBuoiPT, double gia);
    static void xoaGoiTap(const string& maGoi);
    static const MyVector<GoiTap*>& getAllGoiTap();
};
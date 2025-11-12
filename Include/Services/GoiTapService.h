#pragma once
#include <string>
using namespace std;

class GoiTapService {
public:
    static void themGoiTap(const string& tenGoi, int thoiGian, int soBuoiPT, double gia);
    static void themMonTapVaoGoiTap(const string& maGoi, const string& maMon);
    static void suaGoiTap(const string& maGoi, const string& tenGoi, int thoiGian, int soBuoiPT, double gia);
    static void xoaGoiTap(const string& maGoi);
};
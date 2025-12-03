#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class LopHoc;

class LopHocService {
public:
    static void themLopHoc(const string& tenLop, const string& lichTap, int thoiLuong,
                           const string& maMon, const string& maHLV);
    static void suaLopHoc(const string& maLop, const string& tenLop, const string& lichTap,
                         int thoiLuong, const string& maMon, const string& maHLV);
    static void xoaLopHoc(const string& maLop);
    static const MyVector<LopHoc*>& getAllLopHoc();
};
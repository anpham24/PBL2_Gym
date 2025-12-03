#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class HLV;

class HLVService {
public:
    static void themHLV(const string& tenHLV, const string& sdt,
                       const string& gioiTinh, const string& ngaySinh, 
                       const string& chuyenMon, double luong);
    static void suaHLV(const string& maHLV, const string& tenHLV, const string& sdt,
                      const string& gioiTinh, const string& ngaySinh,
                      const string& chuyenMon, double luong);
    static void xoaHLV(const string& maHLV);
    static const MyVector<HLV*>& getAllHLV();
};

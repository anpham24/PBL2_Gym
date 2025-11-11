#pragma once
#include <string>
using namespace std;

class HoiVienService {
public:
    static void themHoiVien(const string& tenHV, const string& sdt,
                            const string& gioiTinh, const string& ngaySinh, int point);
    static void suaHoiVien(const string& maHV, const string& tenHV, const string& sdt,
                               const string& gioiTinh, const string& ngaySinh, int point);
    static void xoaHoiVien(const string& maHV);
};
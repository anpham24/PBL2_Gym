#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class NhanVien;

class NhanVienService {
public:
    static void themNhanVien(const string& tenNV, const string& sdt,
                            const string& gioiTinh, const string& ngaySinh, double luong);
    static void suaNhanVien(const string& maNV, const string& tenNV, const string& sdt,
                           const string& gioiTinh, const string& ngaySinh, double luong);
    static void xoaNhanVien(const string& maNV);
    static const MyVector<NhanVien*>& getAllNhanVien();
};

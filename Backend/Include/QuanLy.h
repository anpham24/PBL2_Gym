#pragma once
#include <string>
#include <fstream>
#include "MyVector.h"
#include "NhanVien.h"
#include "HoiVien.h"
#include "HLV.h"
#include "GoiTap.h"
#include "HangHoa.h"
#include "HoaDon.h"
#include "ThamGia.h"

using namespace std;

class QuanLy {
private:
    // Cac danh sach doi tuong
    MyVector<NhanVien> dsNhanVien;
    MyVector<HoiVien> dsHoiVien;
    MyVector<HLV> dsHLV;
    MyVector<GoiTap> dsGoiTap;
    MyVector<HangHoa> dsHangHoa;
    MyVector<HoaDon> dsHoaDon;
    MyVector<ThamGia> dsThamGia;

    // Cac ham tim index doi tuong trong vector
    int timIndexHoiVien(const string&) const;
    int timIndexNhanVien(const string&) const;
    int timIndexHLV(const string&) const;
    int timIndexGoiTap(const string&) const;
    int timIndexHangHoa(const string&) const;
    int timIndexHoaDon(const string&) const;

public:
    QuanLy();
    ~QuanLy();

    // CRUD NhanVien
    bool themNhanVien(const NhanVien&);
    bool suaNhanVien(const string&, const NhanVien&);
    bool xoaNhanVien(const string&);
    NhanVien* timNhanVien(const string&);
    
};
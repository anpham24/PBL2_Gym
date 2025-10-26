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

using namespace std;

class QuanLy {
private:
    MyVector<NhanVien> dsNhanVien;
    MyVector<HoiVien> dsHoiVien;
    MyVector<HLV> dsHLV;
    MyVector<GoiTap> dsGoiTap;
    MyVector<HangHoa> dsHangHoa;
    MyVector<HoaDon> dsHoaDon;

    
};
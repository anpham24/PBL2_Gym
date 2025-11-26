#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class HangHoa;

class HangHoaService {
public:
    static void themHangHoa(const string& tenHH, const string& loaiHH, double gia, int soLuong);
    static void suaHangHoa(const string& maHH, const string& tenHH, const string& loaiHH, 
                          double gia, int soLuong);
    static void xoaHangHoa(const string& maHH);
    static const MyVector<HangHoa*>& getAllHangHoa();
};

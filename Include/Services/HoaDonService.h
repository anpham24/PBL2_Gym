#pragma once
#include <string>
using namespace std;

class HoaDonService {
public:
    static void taoHoaDon(const string& maNV, const string& maHV, 
                         const string& ngayLap, const string& phuongThucTT);
    static void themHangHoaVaoHoaDon(const string& maHD, const string& maHH, int soLuong, double donGia);
    static void themGoiTapVaoHoaDon(const string& maHD, const string& maGT, int soLuong, double donGia);
    static void xoaHangHoaKhoiHoaDon(const string& maHD, const string& maHH);
    static void xoaGoiTapKhoiHoaDon(const string& maHD, const string& maGT);
};

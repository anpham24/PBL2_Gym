#pragma once
#include <string>
using namespace std;

class GoiTap;
class HoaDon;

class ChiTietHoaDon_GT {
private:
    GoiTap* goiTap; // Pointer to the associated GoiTap
    HoaDon* hoaDon; // Pointer to the associated HoaDon
    int soLuong;
    double donGia;
public:
    ChiTietHoaDon_GT();
    ChiTietHoaDon_GT(GoiTap* goiTap, HoaDon* hoaDon, int soLuong, double donGia);
    ChiTietHoaDon_GT(const ChiTietHoaDon_GT& other);
    ~ChiTietHoaDon_GT();

    GoiTap* getGoiTap() const;
    HoaDon* getHoaDon() const;
    int getSoLuong() const;
    double getDonGia() const;

    void setGoiTap(GoiTap*);
    void setHoaDon(HoaDon*);
    void setSoLuong(int);
    void setDonGia(double);

    static ChiTietHoaDon_GT* create(GoiTap* goiTap, HoaDon* hoaDon, int soLuong, double donGia);
    double tinhTien() const;
    string read() const;
};
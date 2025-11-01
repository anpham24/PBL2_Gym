#pragma once
#include <string>
using namespace std;

class GoiTap;

class ChiTietHoaDon_GT {
private:
    GoiTap* goiTap; // Pointer to the associated GoiTap
    int soLuong;
    double donGia;
public:
    ChiTietHoaDon_GT();
    ChiTietHoaDon_GT(GoiTap* goiTap, int soLuong, double donGia);
    ChiTietHoaDon_GT(const ChiTietHoaDon_GT& other);
    ~ChiTietHoaDon_GT();

    GoiTap* getGoiTap() const;
    int getSoLuong() const;
    double getDonGia() const;

    void setGoiTap(GoiTap*);
    void setSoLuong(int);
    void setDonGia(double);

    static ChiTietHoaDon_GT* create(GoiTap* goiTap, int soLuong, double donGia);
    double tinhTien() const;
    string read() const;
};
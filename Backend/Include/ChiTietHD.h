#pragma once
#include <string>
using namespace std;

class ChiTietHD {
private:
    string maSP; // Gói tập hoặc hàng hóa
    int soLuong;
    double donGia;
public:
    ChiTietHD();
    ChiTietHD(const string& maSP, int soLuong, double donGia);
    ChiTietHD(const ChiTietHD& other);
    ~ChiTietHD();
    // Getters
    const string& getMaSP() const;
    int getSoLuong() const;
    double getDonGia() const;

    // Setters
    void setMaSP(const string& maSP);
    void setSoLuong(int soLuong);
    void setDonGia(double donGia);

    // Logic
    double tinhTien() const;
    string read() const;
};
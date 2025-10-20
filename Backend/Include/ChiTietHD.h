#pragma once
#include <string>
using namespace std;

class ChiTietHD {
    private:
        string maSP;
        string tenSP;
        int soLuong;
        double donGia;
    public:
        ChiTietHD();
        ChiTietHD(const string& maSP, const string& tenSP, int soLuong, double donGia);

        // Getters
        string getMaSP() const;
        string getTenSP() const;
        int getSoLuong() const;
        double getDonGia() const;

        // Logic
        double tinhTien() const;
        string getLoaiSP() const;  // "Hàng hóa" hoặc "Gói tập"
        string read() const;
};
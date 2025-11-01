#pragma once
#include <string>
using namespace std;

class HangHoa;

class ChiTietHoaDon_HH {
private:
    HangHoa* hangHoa; // Pointer to the associated HangHoa
    int soLuong;
    double donGia;
public:
    ChiTietHoaDon_HH();
    ChiTietHoaDon_HH(HangHoa*, int, double);
    ChiTietHoaDon_HH(const ChiTietHoaDon_HH&);
    ~ChiTietHoaDon_HH();
    
    HangHoa* getHangHoa() const;
    int getSoLuong() const;
    double getDonGia() const;

    void setHangHoa(HangHoa*);
    void setSoLuong(int);
    void setDonGia(double);

    static ChiTietHoaDon_HH* create(HangHoa*, int, double);
    double tinhTien() const;
    string read() const;
};
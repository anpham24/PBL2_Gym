#pragma once
#include <string>
using namespace std;

class HangHoa;
class HoaDon;

class ChiTietHoaDon_HH {
private:
    HangHoa* hangHoa; // Pointer to the associated HangHoa
    HoaDon* hoaDon;   // Pointer to the associated HoaDon
    int soLuong;
    double donGia;
public:
    ChiTietHoaDon_HH();
    ChiTietHoaDon_HH(HangHoa*, HoaDon*, int, double);
    ChiTietHoaDon_HH(const ChiTietHoaDon_HH&);
    ~ChiTietHoaDon_HH();
    
    HangHoa* getHangHoa() const;
    HoaDon* getHoaDon() const;
    int getSoLuong() const;
    double getDonGia() const;

    void setHangHoa(HangHoa*);
    void setHoaDon(HoaDon*);
    void setSoLuong(int);
    void setDonGia(double);

    static ChiTietHoaDon_HH* create(HangHoa*, HoaDon*, int, double);
    double tinhTien() const;
    string read() const;
};
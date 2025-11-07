#pragma once
#include <string>
using namespace std;

class HangHoa;
class HoaDon;

class ChiTietHoaDon_HH {
private:
    int soLuong;
    double donGia;
    HoaDon* hoaDon;
    HangHoa* hangHoa;
public:
    ChiTietHoaDon_HH();
    ChiTietHoaDon_HH(int, double, HoaDon*, HangHoa*);
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

    static ChiTietHoaDon_HH* create(int, double, HoaDon*, HangHoa*);
    double tinhTien() const;
    string read() const;
};
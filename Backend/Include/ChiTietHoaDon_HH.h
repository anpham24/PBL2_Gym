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
    ChiTietHoaDon_HH(HangHoa* hangHoa, int soLuong, double donGia);
    ChiTietHoaDon_HH(const ChiTietHoaDon_HH& other);
    ~ChiTietHoaDon_HH();
    
    HangHoa* getHangHoa() const;
    int getSoLuong() const;
    double getDonGia() const;

    void setHangHoa(HangHoa* hangHoa);
    void setSoLuong(int soLuong);
    void setDonGia(double donGia);

    static ChiTietHoaDon_HH* create(HangHoa* hangHoa, int soLuong, double donGia);
    double tinhTien() const;
    string read() const;
};
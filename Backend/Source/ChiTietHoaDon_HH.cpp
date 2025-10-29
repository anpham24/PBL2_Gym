#include "../Include/ChiTietHoaDon_HH.h"
#include "../Include/HangHoa.h"

ChiTietHoaDon_HH::ChiTietHoaDon_HH() {}

ChiTietHoaDon_HH::ChiTietHoaDon_HH(HangHoa* hangHoa, int soLuong, double donGia)
    : hangHoa(hangHoa), soLuong(soLuong), donGia(donGia) {}

ChiTietHoaDon_HH::ChiTietHoaDon_HH(const ChiTietHoaDon_HH& other)
    : hangHoa(other.hangHoa), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHoaDon_HH::~ChiTietHoaDon_HH() {}

HangHoa* ChiTietHoaDon_HH::getHangHoa() const { return hangHoa; }
int ChiTietHoaDon_HH::getSoLuong() const { return soLuong; }
double ChiTietHoaDon_HH::getDonGia() const { return donGia; }

double ChiTietHoaDon_HH::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_HH::read() const {
    string result = hangHoa->getID() + "," + to_string(soLuong) + "," + to_string(donGia) + "," + to_string(tinhTien());
    return result;
}

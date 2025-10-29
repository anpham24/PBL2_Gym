#include "../Include/ChiTietHoaDon_GT.h"
#include "../Include/GoiTap.h"

ChiTietHoaDon_GT::ChiTietHoaDon_GT() {}

ChiTietHoaDon_GT::ChiTietHoaDon_GT(GoiTap* goiTap, int soLuong, double donGia)
    : goiTap(goiTap), soLuong(soLuong), donGia(donGia) {}

ChiTietHoaDon_GT::ChiTietHoaDon_GT(const ChiTietHoaDon_GT& other)
    : goiTap(other.goiTap), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHoaDon_GT::~ChiTietHoaDon_GT() {}

GoiTap* ChiTietHoaDon_GT::getGoiTap() const { return goiTap; }
int ChiTietHoaDon_GT::getSoLuong() const { return soLuong; }
double ChiTietHoaDon_GT::getDonGia() const { return donGia; }

double ChiTietHoaDon_GT::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_GT::read() const {
    string result = goiTap->getID() + "," + to_string(soLuong) + "," + to_string(donGia) + "," + to_string(tinhTien());
    return result;
}

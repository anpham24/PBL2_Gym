#include "ChiTietHD.h"

ChiTietHD::ChiTietHD() {}

ChiTietHD::ChiTietHD(const string& maSP, int soLuong, double donGia)
    : maSP(maSP), soLuong(soLuong), donGia(donGia) {}

ChiTietHD::ChiTietHD(const ChiTietHD& other)
    : maSP(other.maSP), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHD::~ChiTietHD() {}

const string& ChiTietHD::getMaSP() const { return maSP; }
int ChiTietHD::getSoLuong() const { return soLuong; }
double ChiTietHD::getDonGia() const { return donGia; }

double ChiTietHD::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHD::read() const {
    string result = maSP + "," + to_string(soLuong) + "," + to_string(donGia);
    return result;
}

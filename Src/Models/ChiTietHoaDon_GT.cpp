#include "ChiTietHoaDon_GT.h"
#include "GoiTap.h"
#include "HoaDon.h"

ChiTietHoaDon_GT::ChiTietHoaDon_GT() {}

ChiTietHoaDon_GT::ChiTietHoaDon_GT(int soLuong, double donGia, GoiTap* goiTap, HoaDon* hoaDon)
    : goiTap(goiTap), hoaDon(hoaDon), soLuong(soLuong), donGia(donGia) {}

ChiTietHoaDon_GT::ChiTietHoaDon_GT(const ChiTietHoaDon_GT& other)
    : goiTap(other.goiTap), hoaDon(other.hoaDon), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHoaDon_GT::~ChiTietHoaDon_GT() {}

GoiTap* ChiTietHoaDon_GT::getGoiTap() const { return goiTap; }
HoaDon* ChiTietHoaDon_GT::getHoaDon() const { return hoaDon; }
int ChiTietHoaDon_GT::getSoLuong() const { return soLuong; }
double ChiTietHoaDon_GT::getDonGia() const { return donGia; }

void ChiTietHoaDon_GT::setGoiTap(GoiTap* gt) { 
    if (this->goiTap != nullptr) {
        this->goiTap->removeChiTietHoaDon_GT(this);
    }
    this->goiTap = gt;
    if (gt != nullptr) {
        gt->addChiTietHoaDon_GT(this);
    }
}

void ChiTietHoaDon_GT::setHoaDon(HoaDon* hd) {
    if (this->hoaDon != nullptr) {
        this->hoaDon->removeChiTietHoaDon_GT(this);
    }
    this->hoaDon = hd;
    if (hd != nullptr) {
        hd->addChiTietHoaDon_GT(this);
    }
}

ChiTietHoaDon_GT* ChiTietHoaDon_GT::create(int soLuong, double donGia, GoiTap* goiTap, HoaDon* hoaDon) {
    return new ChiTietHoaDon_GT(soLuong, donGia, goiTap, hoaDon);
}

double ChiTietHoaDon_GT::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_GT::read() const {
    string result = goiTap->getID() + "," + to_string(soLuong) + "," + to_string(donGia) + "," + to_string(tinhTien());
    return result;
}

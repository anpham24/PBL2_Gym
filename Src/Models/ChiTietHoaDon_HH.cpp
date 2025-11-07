#include "ChiTietHoaDon_HH.h"
#include "HangHoa.h"
#include "HoaDon.h"

ChiTietHoaDon_HH::ChiTietHoaDon_HH() {}

ChiTietHoaDon_HH::ChiTietHoaDon_HH(int soLuong, double donGia, HangHoa* hangHoa, HoaDon* hoaDon)
    : hangHoa(hangHoa), hoaDon(hoaDon), soLuong(soLuong), donGia(donGia) {}

ChiTietHoaDon_HH::ChiTietHoaDon_HH(const ChiTietHoaDon_HH& other)
    : hangHoa(other.hangHoa), hoaDon(other.hoaDon), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHoaDon_HH::~ChiTietHoaDon_HH() {}

HangHoa* ChiTietHoaDon_HH::getHangHoa() const { return hangHoa; }
HoaDon* ChiTietHoaDon_HH::getHoaDon() const { return hoaDon; }
int ChiTietHoaDon_HH::getSoLuong() const { return soLuong; }
double ChiTietHoaDon_HH::getDonGia() const { return donGia; }

void ChiTietHoaDon_HH::setHangHoa(HangHoa* hh) { 
    if (this->hangHoa != nullptr) {
        this->hangHoa->removeChiTietHoaDon_HH(this);
    }
    this->hangHoa = hh;
    if (hh != nullptr) {
        hh->addChiTietHoaDon_HH(this);
    }
}
void ChiTietHoaDon_HH::setHoaDon(HoaDon* hd) {
    if (this->hoaDon != nullptr) {
        this->hoaDon->removeChiTietHoaDon_HH(this);
    }
    this->hoaDon = hd;
    if (hd != nullptr) {
        hd->addChiTietHoaDon_HH(this);
    }
}
void ChiTietHoaDon_HH::setSoLuong(int sl) { soLuong = sl; }
void ChiTietHoaDon_HH::setDonGia(double dg) { donGia = dg; }

ChiTietHoaDon_HH* ChiTietHoaDon_HH::create(int soLuong, double donGia, HangHoa* hangHoa, HoaDon* hoaDon) {
    return new ChiTietHoaDon_HH(soLuong, donGia, hangHoa, hoaDon);
}

double ChiTietHoaDon_HH::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_HH::read() const {
    string result = hangHoa->getID() + "," + to_string(soLuong) + "," + to_string(donGia) + "," + to_string(tinhTien());
    return result;
}

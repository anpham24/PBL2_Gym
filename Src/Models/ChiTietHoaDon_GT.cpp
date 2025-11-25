#include "ChiTietHoaDon_GT.h"
#include "GoiTap.h"
#include "HoaDon.h"

ChiTietHoaDon_GT::ChiTietHoaDon_GT() {}

ChiTietHoaDon_GT::ChiTietHoaDon_GT(int soLuong, double donGia, HoaDon* hoaDon, GoiTap* goiTap)
    : goiTap(goiTap), hoaDon(hoaDon), soLuong(soLuong), donGia(donGia) {}

ChiTietHoaDon_GT::ChiTietHoaDon_GT(const ChiTietHoaDon_GT& other)
    : goiTap(other.goiTap), hoaDon(other.hoaDon), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHoaDon_GT::~ChiTietHoaDon_GT() {}

GoiTap* ChiTietHoaDon_GT::getGoiTap() const { return goiTap; }
HoaDon* ChiTietHoaDon_GT::getHoaDon() const { return hoaDon; }
int ChiTietHoaDon_GT::getSoLuong() const { return soLuong; }
double ChiTietHoaDon_GT::getDonGia() const { return donGia; }

// Cac setter voi quan ly lien ket hai chieu
void ChiTietHoaDon_GT::setGoiTap(GoiTap* gt) { 
    // Xoa khoi danh sach GoiTap cu neu ton tai
    if (this->goiTap != nullptr) {
        this->goiTap->removeChiTietHoaDon_GT(this);
    }
    // Cap nhat con tro
    this->goiTap = gt;
    // Them vao danh sach GoiTap moi neu khong null
    if (gt != nullptr) {
        gt->addChiTietHoaDon_GT(this);
    }
}

void ChiTietHoaDon_GT::setHoaDon(HoaDon* hd) {
    // Xoa khoi danh sach HoaDon cu neu ton tai
    if (this->hoaDon != nullptr) {
        this->hoaDon->removeChiTietHoaDon_GT(this);
    }
    // Cap nhat con tro
    this->hoaDon = hd;
    // Them vao danh sach HoaDon moi neu khong null
    if (hd != nullptr) {
        hd->addChiTietHoaDon_GT(this);
    }
}

ChiTietHoaDon_GT* ChiTietHoaDon_GT::create(int soLuong, double donGia, HoaDon* hoaDon, GoiTap* goiTap) {
    return new ChiTietHoaDon_GT(soLuong, donGia, hoaDon, goiTap);
}

double ChiTietHoaDon_GT::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_GT::read() const {
    string hoaDonID = (hoaDon != nullptr) ? hoaDon->getID() : "NULL";
    string goiTapID = (goiTap != nullptr) ? goiTap->getID() : "NULL";
    string result = hoaDonID + ";" + goiTapID + ";" + to_string(soLuong) + ";" + to_string(donGia);
    return result;
}

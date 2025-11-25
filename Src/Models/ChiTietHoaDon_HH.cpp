#include "ChiTietHoaDon_HH.h"
#include "HangHoa.h"
#include "HoaDon.h"

ChiTietHoaDon_HH::ChiTietHoaDon_HH() {}

ChiTietHoaDon_HH::ChiTietHoaDon_HH(int soLuong, double donGia, HoaDon* hoaDon, HangHoa* hangHoa)
    : hangHoa(hangHoa), hoaDon(hoaDon), soLuong(soLuong), donGia(donGia) {}

ChiTietHoaDon_HH::ChiTietHoaDon_HH(const ChiTietHoaDon_HH& other)
    : hangHoa(other.hangHoa), hoaDon(other.hoaDon), soLuong(other.soLuong), donGia(other.donGia) {}

ChiTietHoaDon_HH::~ChiTietHoaDon_HH() {}

HangHoa* ChiTietHoaDon_HH::getHangHoa() const { return hangHoa; }
HoaDon* ChiTietHoaDon_HH::getHoaDon() const { return hoaDon; }
int ChiTietHoaDon_HH::getSoLuong() const { return soLuong; }
double ChiTietHoaDon_HH::getDonGia() const { return donGia; }

// Cac setter voi quan ly lien ket hai chieu
void ChiTietHoaDon_HH::setHangHoa(HangHoa* hh) { 
    // Xoa khoi danh sach HangHoa cu neu ton tai
    if (this->hangHoa != nullptr) {
        this->hangHoa->removeChiTietHoaDon_HH(this);
    }
    // Cap nhat con tro
    this->hangHoa = hh;
    // Them vao danh sach HangHoa moi neu khong null
    if (hh != nullptr) {
        hh->addChiTietHoaDon_HH(this);
    }
}

void ChiTietHoaDon_HH::setHoaDon(HoaDon* hd) {
    // Xoa khoi danh sach HoaDon cu neu ton tai
    if (this->hoaDon != nullptr) {
        this->hoaDon->removeChiTietHoaDon_HH(this);
    }
    // Cap nhat con tro
    this->hoaDon = hd;
    // Them vao danh sach HoaDon moi neu khong null
    if (hd != nullptr) {
        hd->addChiTietHoaDon_HH(this);
    }
}
void ChiTietHoaDon_HH::setSoLuong(int sl) { soLuong = sl; }
void ChiTietHoaDon_HH::setDonGia(double dg) { donGia = dg; }

ChiTietHoaDon_HH* ChiTietHoaDon_HH::create(int soLuong, double donGia, HoaDon* hoaDon, HangHoa* hangHoa) {
    return new ChiTietHoaDon_HH(soLuong, donGia, hoaDon, hangHoa);
}

double ChiTietHoaDon_HH::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_HH::read() const {
    string hoaDonID = (hoaDon != nullptr) ? hoaDon->getID() : "NULL";
    string hangHoaID = (hangHoa != nullptr) ? hangHoa->getID() : "NULL";
    string result = hoaDonID + ";" + hangHoaID + ";" + to_string(soLuong) + ";" + to_string(donGia);
    return result;
}

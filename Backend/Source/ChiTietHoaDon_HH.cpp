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

void ChiTietHoaDon_HH::setHangHoa(HangHoa* hh) { 
    if (this->hangHoa != nullptr) {
        this->hangHoa->removeChiTietHoaDon_HH(this);
    }
    this->hangHoa = hh;
    if (hh != nullptr) {
        hh->addChiTietHoaDon_HH(this);
    }
}
void ChiTietHoaDon_HH::setSoLuong(int sl) { soLuong = sl; }
void ChiTietHoaDon_HH::setDonGia(double dg) { donGia = dg; }

ChiTietHoaDon_HH* ChiTietHoaDon_HH::create(HangHoa* hangHoa, int soLuong, double donGia) {
    return new ChiTietHoaDon_HH(hangHoa, soLuong, donGia);
}

double ChiTietHoaDon_HH::tinhTien() const {
    return soLuong * donGia;
}

string ChiTietHoaDon_HH::read() const {
    string result = hangHoa->getID() + "," + to_string(soLuong) + "," + to_string(donGia) + "," + to_string(tinhTien());
    return result;
}

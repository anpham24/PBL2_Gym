#include "../Include/HLV.h"
#include <sstream>

HLV::HLV() {
    this->ID = generateID(30);
}

HLV::HLV(const string& hoTen, const string& sdt, const string& gioiTinh, double luong)
    : Person(hoTen, sdt, gioiTinh), luong(luong) {
    this->ID = generateID(30);
}

HLV::~HLV() {

}

double HLV::getLuong() const {
    return this->luong;
}

void HLV::setLuong(double luong) {
    this->luong = luong;
}

void HLV::create(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    *this = HLV(hoTen, sdt, gioiTinh, luong);
}

string HLV::read() const{
    ostringstream oss;
    oss << ID << "," << hoTen << "," << sdt << "," << gioiTinh << "," << luong;
    return oss.str();
}

void HLV::update(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->luong = luong;
}
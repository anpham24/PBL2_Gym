#include "../Include/HLV.h"
#include "../Include/IDGenerator.h"
#define HLV_ID 3

HLV::HLV() {
    this->id = IDGenerator::generateID(HLV_ID);
}

HLV::HLV(const string& hoTen, const string& sdt, const string& gioiTinh, double luong)
    : Person(hoTen, sdt, gioiTinh), luong(luong) {
    this->id = IDGenerator::generateID(HLV_ID);
}

HLV::HLV(const HLV& other) 
    : Person(other.hoTen, other.sdt, other.gioiTinh), luong(other.luong) {
    this->id = other.id;
}

HLV::~HLV() {}

double HLV::getLuong() const { return this->luong; }

void HLV::setLuong(double luong) { this->luong = luong; }

HLV HLV::create(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    return HLV(hoTen, sdt, gioiTinh, luong);
}

string HLV::read() const {
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + to_string(luong);
    return result;
}

void HLV::update(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->luong = luong;
}
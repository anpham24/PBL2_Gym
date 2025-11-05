#include "HLV.h"
#include "HoiVien.h"
#include "IDGenerator.h"

HLV::HLV() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
}

HLV::HLV(const string& hoTen, const string& sdt, const string& gioiTinh, int tuoi, double luong)
    : Person(hoTen, sdt, gioiTinh, tuoi), luong(luong) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
}

HLV::HLV(const HLV& other) 
    : Person(other.hoTen, other.sdt, other.gioiTinh, other.tuoi), luong(other.luong) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
}

HLV::~HLV() {}

double HLV::getLuong() const { return this->luong; }
void HLV::setLuong(double luong) { this->luong = luong; }

void HLV::addHoiVien(HoiVien* hv) {
    dsHoiVien.push_back(hv);
}

void HLV::removeHoiVien(HoiVien* hv) {
    for (int i = 0; i < dsHoiVien.size(); i++) {
        if (dsHoiVien[i] == hv) {
            dsHoiVien.erase(i);
            break;
        }
    }
}

const MyVector<HoiVien*>& HLV::getDsHoiVien() const {
    return dsHoiVien;
}

MyVector<HoiVien*>& HLV::getDsHoiVien() {
    return dsHoiVien;
}

void HLV::addLopHoc(LopHoc* lh) {
    dsLopHoc.push_back(lh);
}

void HLV::removeLopHoc(LopHoc* lh) {
    for (int i = 0; i < dsLopHoc.size(); i++) {
        if (dsLopHoc[i] == lh) {
            dsLopHoc.erase(i);
            break;
        }
    }
}

const MyVector<LopHoc*>& HLV::getDsLopHoc() const {
    return dsLopHoc;
}

MyVector<LopHoc*>& HLV::getDsLopHoc() {
    return dsLopHoc;
}

HLV* HLV::create(const string& hoTen, const string& sdt, const string& gioiTinh, int tuoi, double luong) {
    return new HLV(hoTen, sdt, gioiTinh, tuoi, luong);
}

string HLV::read() const {
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + to_string(tuoi) + "," + to_string(luong); 
    return result;
}

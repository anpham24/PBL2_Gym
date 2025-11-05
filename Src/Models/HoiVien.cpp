#include "HoiVien.h"
#include "HLV.h"
#include "HopDong.h"
#include "IDGenerator.h"

HoiVien::HoiVien() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HoiVien);
}

HoiVien::HoiVien(const string& hoTen, const string& sdt, const string& gioiTinh, int tuoi, int point, HLV* hlv, bool isActive)
    : Person(hoTen, sdt, gioiTinh, tuoi), point(point), hlv(hlv), isActive(isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HoiVien);
}

HoiVien::HoiVien(const HoiVien& other) 
    : Person(other.hoTen, other.sdt, other.gioiTinh, other.tuoi), point(other.point), hlv(other.hlv), isActive(other.isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HoiVien);
}

HoiVien::~HoiVien() {}

void HoiVien::setPoint(int point) { this->point = point; }

void HoiVien::setHLV(HLV* newHLV) {
    if (this->hlv != nullptr) {
        this->hlv->removeHoiVien(this);
    }
    this->hlv = newHLV;
    if (newHLV != nullptr) {
        newHLV->addHoiVien(this);
    }
}

double HoiVien::getPoint() const { return this->point; }
const HLV* HoiVien::getHLV() const { return this->hlv; }   

bool HoiVien::getIsActive() const { return this->isActive; }
void HoiVien::setIsActive(bool isActive) { this->isActive = isActive; } 

void HoiVien::addHoaDon(HoaDon* hoaDon) {
    dsHoaDon.push_back(hoaDon);
}

void HoiVien::removeHoaDon(HoaDon* hoaDon) {
    for (int i = 0; i < dsHoaDon.size(); i++) {
        if (dsHoaDon[i] == hoaDon) {
            dsHoaDon.erase(i);
            break;
        }
    }
}

const MyVector<HoaDon*>& HoiVien::getDsHoaDon() const {
    return dsHoaDon;
}

MyVector<HoaDon*>& HoiVien::getDsHoaDon() {
    return dsHoaDon;
}

void HoiVien::addHopDong(HopDong* hopDong) {
    dsHopDong.push_back(hopDong);
}

void HoiVien::removeHopDong(HopDong* hopDong) {
    for (int i = 0; i < dsHopDong.size(); i++) {
        if (dsHopDong[i] == hopDong) {
            dsHopDong.erase(i);
            break;
        }
    }
}

const MyVector<HopDong*>& HoiVien::getDsHopDong() const {
    return dsHopDong;
}

MyVector<HopDong*>& HoiVien::getDsHopDong() {
    return dsHopDong;
}

HoiVien* HoiVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, int tuoi, int point, HLV* hlv, bool isActive) {
    return new HoiVien(hoTen, sdt, gioiTinh, tuoi, point, hlv, isActive);
}

string HoiVien::read() const{
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + to_string(tuoi) + "," + to_string(point) + "," + (hlv ? hlv->getID() : "null") + "," + (isActive ? "true" : "false");
    return result;
}

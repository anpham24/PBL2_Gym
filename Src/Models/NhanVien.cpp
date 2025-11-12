#include "NhanVien.h"
#include "HopDong.h"
#include "HoaDon.h"
#include "IDGenerator.h"

NhanVien::NhanVien() {
}

NhanVien::NhanVien(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, double luong, bool isActive)
    : Person(id, hoTen, sdt, gioiTinh, ngaySinh), luong(luong), isActive(isActive) {}

NhanVien::NhanVien(const NhanVien& other)
    : Person(other.id, other.hoTen, other.sdt, other.gioiTinh, other.ngaySinh), luong(other.luong), isActive(other.isActive) {}

NhanVien::~NhanVien() {}

// Getters
double NhanVien::getLuong() const { return this->luong; }
bool NhanVien::getIsActive() const { return this->isActive; }

// Setters
void NhanVien::setLuong(double luong) { this->luong = luong; }
void NhanVien::setIsActive(bool isActive) { this->isActive = isActive; }

void NhanVien::addHopDong(HopDong* hopDong) {
    this->dsHopDong.push_back(hopDong);
}

void NhanVien::removeHopDong(HopDong* hopDong) {
    for (size_t i = 0; i < dsHopDong.size(); i++) {
        if (dsHopDong[i] == hopDong) {
            dsHopDong.erase(i);
            return;
        }
    }
}

const MyVector<HopDong*>& NhanVien::getDsHopDong() const {
    return this->dsHopDong;
}

MyVector<HopDong*>& NhanVien::getDsHopDong() {
    return this->dsHopDong;
}

void NhanVien::addHoaDon(HoaDon* hoaDon) {
    this->dsHoaDon.push_back(hoaDon);
}

void NhanVien::removeHoaDon(HoaDon* hoaDon) {
    for (size_t i = 0; i < dsHoaDon.size(); i++) {
        if (dsHoaDon[i] == hoaDon) {
            dsHoaDon.erase(i);
            return;
        }
    }
}

const MyVector<HoaDon*>& NhanVien::getDsHoaDon() const {
    return this->dsHoaDon;
}

MyVector<HoaDon*>& NhanVien::getDsHoaDon() {
    return this->dsHoaDon;
}

NhanVien* NhanVien::create(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, double luong, bool isActive) {
    return new NhanVien(id, hoTen, sdt, gioiTinh, ngaySinh, luong, isActive);
}

NhanVien* NhanVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, double luong, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_NhanVien);
    return new NhanVien(id, hoTen, sdt, gioiTinh, ngaySinh, luong, isActive);
}

string NhanVien::read() const {
    string result = id + ";" + hoTen + ";" + sdt + ";" + gioiTinh + ";" + ngaySinh + ";" + to_string(luong) + ";" + (isActive ? "true" : "false");
    return result;
}

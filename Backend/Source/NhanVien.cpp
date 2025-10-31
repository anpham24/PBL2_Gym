#include "../Include/NhanVien.h"
#include "../Include/HopDong.h"
#include "../Include/HoaDon.h"
#include "../Include/IDGenerator.h"

NhanVien::NhanVien() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_NhanVien);
}

NhanVien::NhanVien(const string& hoTen, const string& sdt, const string& gioiTinh, int tuoi, double luong)
    : Person(hoTen, sdt, gioiTinh, tuoi), luong(luong) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_NhanVien);
}

NhanVien::NhanVien(const NhanVien& other)
    : Person(other.hoTen, other.sdt, other.gioiTinh, other.tuoi), luong(other.luong) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_NhanVien);
}

NhanVien::~NhanVien() {}

double NhanVien::getLuong() const { return this->luong; }
void NhanVien::setLuong(double luong) { this->luong = luong; }

void NhanVien::addHopDong(HopDong* hopDong) {
    this->dsHopDong.push_back(hopDong);
}

void NhanVien::removeHopDong(HopDong* hopDong) {
    for (size_t i = 0; i < dsHopDong.size(); ++i) {
        if (dsHopDong.at(i) == hopDong) {
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
    for (size_t i = 0; i < dsHoaDon.size(); ++i) {
        if (dsHoaDon.at(i) == hoaDon) {
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

NhanVien* NhanVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, int tuoi, double luong) {
    return new NhanVien(hoTen, sdt, gioiTinh, tuoi, luong);
}


string NhanVien::read() const {
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + to_string(tuoi) + "," + to_string(luong);
    return result;
}

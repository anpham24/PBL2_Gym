#include "../Include/NhanVien.h"
#include "../Include/IDGenerator.h"
#define NV_ID 1

NhanVien::NhanVien() {
    this->id = IDGenerator::generateID(NV_ID);
}

NhanVien::NhanVien(const string& hoTen, const string& sdt, const string& gioiTinh, double luong)
    : Person(hoTen, sdt, gioiTinh), luong(luong) {
    this->id = IDGenerator::generateID(NV_ID);
}

NhanVien::NhanVien(const NhanVien& other)
    : Person(other.hoTen, other.sdt, other.gioiTinh), luong(other.luong) {
    this->id = other.id;
}

NhanVien::~NhanVien() {}

double NhanVien::getLuong() const { return this->luong; }

void NhanVien::setLuong(double luong) { this->luong = luong; }

NhanVien NhanVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    return NhanVien(hoTen, sdt, gioiTinh, luong);
}


string NhanVien::read() const {
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + to_string(luong);
    return result;
}

void NhanVien::update(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->luong = luong;
}
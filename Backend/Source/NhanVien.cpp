#include "../Include/NhanVien.h"
#include "../Include/IDGenerator.h"
#include <sstream>

NhanVien::NhanVien() {
    this->id = IDGenerator::generateID(10);
}

NhanVien::NhanVien(const string& hoTen, const string& sdt, const string& gioiTinh, double luong)
    : Person(hoTen, sdt, gioiTinh), luong(luong) {
    this->id = IDGenerator::generateID(10);
}

NhanVien::~NhanVien() {

}

double NhanVien::getLuong() const { return this->luong; }

void NhanVien::setLuong(double luong) { this->luong = luong; }

NhanVien NhanVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    return NhanVien(hoTen, sdt, gioiTinh, luong);
}

string NhanVien::read() const{
    ostringstream oss;
    oss << id << "," << hoTen << "," << sdt << "," << gioiTinh << "," << luong;
    return oss.str();
}

void NhanVien::update(const string& hoTen, const string& sdt, const string& gioiTinh, double luong) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->luong = luong;
}
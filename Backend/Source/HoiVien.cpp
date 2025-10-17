#include "../Include/HoiVien.h"
#include <sstream>

HoiVien::HoiVien() {
    this->ID = generateID(20);
}

HoiVien::HoiVien(const string& hoTen, const string& sdt, const string& gioiTinh, int Point)
    : Person(hoTen, sdt, gioiTinh), point(point) {
    this->ID = generateID(20);
}

HoiVien::~HoiVien() {

}

double HoiVien::getPoint() const {
    return this->point;
}

void HoiVien::setPoint(int Point) {
    this->point = point;
}

void HoiVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, int point) {
    *this = HoiVien(hoTen, sdt, gioiTinh, point);
}

string HoiVien::read() const{
    ostringstream oss;
    oss << ID << "," << hoTen << "," << sdt << "," << gioiTinh << "," << point;
    return oss.str();
}

void HoiVien::update(const string& hoTen, const string& sdt, const string& gioiTinh, int point) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->point = point;
}
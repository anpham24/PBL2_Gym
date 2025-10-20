#include "../Include/HoiVien.h"
#include "../Include/IDGenerator.h"
#include <sstream>

HoiVien::HoiVien() {
    this->id = IDGenerator::generateID(20);
}

HoiVien::HoiVien(const string& hoTen, const string& sdt, const string& gioiTinh, int point, const string& maHLV)
    : Person(hoTen, sdt, gioiTinh), point(point), maHLV(maHLV) {
    this->id = IDGenerator::generateID(20);
}

HoiVien::~HoiVien() {

}

double HoiVien::getPoint() const { return this->point; }

void HoiVien::setPoint(int Point) { this->point = point; }

HoiVien HoiVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, int point, const string& maHLV) {
    return HoiVien(hoTen, sdt, gioiTinh, point, maHLV);
}

string HoiVien::read() const{
    ostringstream oss;
    oss << id << "," << hoTen << "," << sdt << "," << gioiTinh << "," << point <<"," << maHLV;
    return oss.str();
}

void HoiVien::update(const string& hoTen, const string& sdt, const string& gioiTinh, int point, const string& maHLV) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->point = point;
    this->maHLV = maHLV;
}
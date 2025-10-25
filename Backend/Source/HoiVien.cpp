#include "../Include/HoiVien.h"
#include "../Include/IDGenerator.h"
#define HV_ID 2

HoiVien::HoiVien() {
    this->id = IDGenerator::generateID(HV_ID);
}

HoiVien::HoiVien(const string& hoTen, const string& sdt, const string& gioiTinh, int point, const string& maHLV)
    : Person(hoTen, sdt, gioiTinh), point(point), maHLV(maHLV) {
    this->id = IDGenerator::generateID(HV_ID);
}

HoiVien::HoiVien(const HoiVien& other) 
    : Person(other.hoTen, other.sdt, other.gioiTinh), point(other.point), maHLV(other.maHLV) {
    this->id = other.id;
}

HoiVien::~HoiVien() {}

double HoiVien::getPoint() const { return this->point; }

void HoiVien::setPoint(int Point) { this->point = point; }

HoiVien HoiVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, int point, const string& maHLV) {
    return HoiVien(hoTen, sdt, gioiTinh, point, maHLV);
}

string HoiVien::read() const{
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + to_string(point) + "," + maHLV;
    return result;
}

void HoiVien::update(const string& hoTen, const string& sdt, const string& gioiTinh, int point, const string& maHLV) {
    this->hoTen = hoTen;
    this->sdt = sdt;
    this->gioiTinh = gioiTinh;
    this->point = point;
    this->maHLV = maHLV;
}
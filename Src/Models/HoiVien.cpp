#include "HoiVien.h"
#include "HopDong.h"
#include "HoaDon.h"
#include "IDGenerator.h"

HoiVien::HoiVien() {}

HoiVien::HoiVien(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, int point, int soBuoiPT, bool isActive)
    : Person(id, hoTen, sdt, gioiTinh, ngaySinh), point(point), soBuoiPT(soBuoiPT), isActive(isActive) {}

HoiVien::HoiVien(const HoiVien& other) 
    : Person(other.id, other.hoTen, other.sdt, other.gioiTinh, other.ngaySinh), point(other.point), soBuoiPT(other.soBuoiPT), isActive(other.isActive) {}

HoiVien::~HoiVien() {}

int HoiVien::getSoBuoiPT() const { return this->soBuoiPT; }
void HoiVien::setSoBuoiPT(int soBuoiPT) { this->soBuoiPT = soBuoiPT; }

void HoiVien::setPoint(int point) { this->point = point; }
int HoiVien::getPoint() const { return this->point; }

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

void HoiVien::addLogTapPT(LogTapPT* logTapPT) {
    dsLogTapPT.push_back(logTapPT);
}

void HoiVien::removeLogTapPT(LogTapPT* logTapPT) {
    for (int i = 0; i < dsLogTapPT.size(); i++) {
        if (dsLogTapPT[i] == logTapPT) {
            dsLogTapPT.erase(i);
            break;
        }
    }
}

const MyVector<LogTapPT*>& HoiVien::getDsLogTapPT() const {
    return dsLogTapPT;
}

MyVector<LogTapPT*>& HoiVien::getDsLogTapPT() {
    return dsLogTapPT;
}

HoiVien* HoiVien::create(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, int point, int soBuoiPT, bool isActive) {
    return new HoiVien(id, hoTen, sdt, gioiTinh, ngaySinh, point, soBuoiPT, isActive);
}

HoiVien* HoiVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, int point, int soBuoiPT, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HoiVien);
    return new HoiVien(id, hoTen, sdt, gioiTinh, ngaySinh, point, soBuoiPT, isActive);
}

string HoiVien::read() const{
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + ngaySinh + "," + to_string(point) + "," + to_string(soBuoiPT) + "," + (isActive ? "true" : "false");
    return result;
}

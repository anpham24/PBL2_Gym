#include "HLV.h"
#include "LopHoc.h"
#include "LogTapPT.h"
#include "IDGenerator.h"

HLV::HLV() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
}

HLV::HLV(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, const string& chuyenMon, double luong, bool isActive)
    : Person(id, hoTen, sdt, gioiTinh, ngaySinh), chuyenMon(chuyenMon), luong(luong), isActive(isActive) {}

HLV::HLV(const HLV& other) 
    : Person(other.id, other.hoTen, other.sdt, other.gioiTinh, other.ngaySinh), chuyenMon(other.chuyenMon), luong(other.luong), isActive(other.isActive) {}

HLV::~HLV() {}

string HLV::getChuyenMon() const { return this->chuyenMon; }
void HLV::setChuyenMon(const string& chuyenMon) { this->chuyenMon = chuyenMon; }

double HLV::getLuong() const { return this->luong; }
void HLV::setLuong(double luong) { this->luong = luong; }

bool HLV::getIsActive() const { return this->isActive; }
void HLV::setIsActive(bool status) { this->isActive = status; }

void HLV::addLogTapPT(LogTapPT* log) {
    dsLogTapPT.push_back(log);
}

void HLV::removeLogTapPT(LogTapPT* log) {
    for (int i = 0; i < dsLogTapPT.size(); i++) {
        if (dsLogTapPT[i] == log) {
            dsLogTapPT.erase(i);
            break;
        }
    }
}

const MyVector<LogTapPT*>& HLV::getDsLogTapPT() const {
    return dsLogTapPT;
}

MyVector<LogTapPT*>& HLV::getDsLogTapPT() {
    return dsLogTapPT;
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

HLV* HLV::create(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, const string& chuyenMon, double luong, bool isActive) {
    return new HLV(id, hoTen, sdt, gioiTinh, ngaySinh, chuyenMon, luong, isActive);
}

HLV* HLV::create(const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, const string& chuyenMon, double luong, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
    return new HLV(id, hoTen, sdt, gioiTinh, ngaySinh, chuyenMon, luong, isActive);
}

string HLV::read() const {
    string result = id + ";" + hoTen + ";" + sdt + ";" + gioiTinh + ";" + ngaySinh + ";" + chuyenMon + ";" + to_string(luong) + ";" + (isActive ? "true" : "false");
    return result;
}

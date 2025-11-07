#include "HLV.h"
#include "MonTap.h"
#include "LopHoc.h"
#include "LogTapPT.h"
#include "IDGenerator.h"

HLV::HLV() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
}

HLV::HLV(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, double luong, bool isActive)
    : Person(id, hoTen, sdt, gioiTinh, ngaySinh), luong(luong), isActive(isActive) {}

HLV::HLV(const HLV& other) 
    : Person(other.id, other.hoTen, other.sdt, other.gioiTinh, other.ngaySinh), luong(other.luong), isActive(other.isActive) {}

HLV::~HLV() {}

double HLV::getLuong() const { return this->luong; }
void HLV::setLuong(double luong) { this->luong = luong; }

bool HLV::getIsActive() const { return this->isActive; }
void HLV::setIsActive(bool status) { this->isActive = status; }

void HLV::addMonTap(MonTap* mt) {
    dsMonTap.push_back(mt);
}

void HLV::removeMonTap(MonTap* mt) {
    for (int i = 0; i < dsMonTap.size(); i++) {
        if (dsMonTap[i] == mt) {
            dsMonTap.erase(i);
            break;
        }
    }
}

const MyVector<MonTap*>& HLV::getDsMonTap() const {
    return dsMonTap;
}

MyVector<MonTap*>& HLV::getDsMonTap() {
    return dsMonTap;
}

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

HLV* HLV::create(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, double luong, bool isActive) {
    return new HLV(id, hoTen, sdt, gioiTinh, ngaySinh, luong, isActive);
}

HLV* HLV::create(const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, double luong, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HLV);
    return new HLV(id, hoTen, sdt, gioiTinh, ngaySinh, luong, isActive);
}

string HLV::read() const {
    string result = id + "," + hoTen + "," + sdt + "," + gioiTinh + "," + ngaySinh + "," + to_string(luong) + "," + to_string(isActive);
    return result;
}

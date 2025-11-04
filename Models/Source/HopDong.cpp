#include "../Include/HopDong.h"
#include "../Include/HoiVien.h"
#include "../Include/GoiTap.h"
#include "../Include/NhanVien.h"
#include "../../Utils/Include/IDGenerator.h"
#include <ctime>

HopDong::HopDong() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
}

HopDong::HopDong(HoiVien* hv, GoiTap* gt, NhanVien* nv, const string& ngayDK, const string& ngayHetHan, bool isActive)
    : hv(hv), gt(gt), nv(nv), ngayDK(ngayDK), ngayHetHan(ngayHetHan), isActive(isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
}

HopDong::HopDong(const HopDong& other)
    : hv(other.hv), gt(other.gt), nv(other.nv), ngayDK(other.ngayDK), ngayHetHan(other.ngayHetHan), isActive(other.isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
}

HopDong::~HopDong() {}

const string& HopDong::getID() const { return this->id; }
const HoiVien& HopDong::getHoiVien() const { return *hv; }
const GoiTap& HopDong::getGoiTap() const { return *gt; }
const NhanVien& HopDong::getNhanVien() const { return *nv; }
const string& HopDong::getNgayDK() const { return ngayDK; }
const string& HopDong::getNgayHetHan() const { return ngayHetHan; }
bool HopDong::getIsActive() { 
    tm t = {};

    t.tm_mday = stoi(this->ngayHetHan.substr(0, 2));   // 2 ký tự đầu -> ngày
    t.tm_mon  = stoi(this->ngayHetHan.substr(3, 2)) - 1; // 2 ký tự sau dấu '/' -> tháng (bắt đầu từ 0)
    t.tm_year = stoi(this->ngayHetHan.substr(6, 4)) - 1900; // 4 ký tự cuối -> năm (tính từ 1900)

    time_t ngayHetHanEpoch = mktime(&t);
    time_t now = time(nullptr);

    if (ngayHetHanEpoch < now)
        this->isActive = false;

    return this->isActive;
}

void HopDong::setHoiVien(HoiVien* hv) { 
    if (this->hv != nullptr) {
        this->hv->removeHopDong(this);
    }
    this->hv = hv;
    if (hv != nullptr) {
        hv->addHopDong(this);
    }
}

void HopDong::setGoiTap(GoiTap* gt) { this->gt = gt; }
void HopDong::setNhanVien(NhanVien* nv) { 
    if (this->nv != nullptr) {
        this->nv->removeHopDong(this);
    }
    this->nv = nv;
    if (nv != nullptr) {
        nv->addHopDong(this);
    }
}

void HopDong::setNgayDK(const string& ngay) { ngayDK = ngay; }
void HopDong::setNgayHetHan(const string& ngay) { ngayHetHan = ngay; }
void HopDong::setIsActive(bool active) { isActive = active; }

HopDong* HopDong::create(HoiVien* hv, GoiTap* gt, NhanVien* nv,
                   const string& ngayDK, const string& ngayHetHan, bool isActive)
{
    return new HopDong(hv, gt, nv, ngayDK, ngayHetHan, isActive);
}

string HopDong::read() {
    string result = hv->getID() + "," + gt->getID() + "," + nv->getID() + "," + ngayDK + "," + ngayHetHan + "," + to_string(this->getIsActive());
    return result;
}

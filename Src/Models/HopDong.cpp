#include "HopDong.h"
#include "HoiVien.h"
#include "GoiTap.h"
#include "NhanVien.h"
#include "IDGenerator.h"
#include <ctime>

HopDong::HopDong() {}

HopDong::HopDong(const string& id, const string& ngayDK, const string& ngayHetHan, bool isActive, HoiVien* hv, GoiTap* gt, NhanVien* nv)
    : id(id), hv(hv), gt(gt), nv(nv), ngayDK(ngayDK), ngayHetHan(ngayHetHan), isActive(isActive) {}

HopDong::HopDong(const HopDong& other)
    : id(other.id), hv(other.hv), gt(other.gt), nv(other.nv), ngayDK(other.ngayDK), ngayHetHan(other.ngayHetHan), isActive(other.isActive) {}

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
    if (hv == this->hv) return;
    if (this->hv != nullptr) {
        this->hv->removeHopDong(this);
    }
    this->hv = hv;
    if (hv != nullptr) {
        hv->addHopDong(this);
    }
}

void HopDong::setGoiTap(GoiTap* gt) { 
    if (this->gt == gt) return;
    if (this->gt != nullptr) {
        this->gt->removeHopDong(this);
    }
    this->gt = gt;
    if (gt != nullptr) {
        gt->addHopDong(this);
    }
}

void HopDong::setNhanVien(NhanVien* nv) { 
    if (this->nv == nv) return;
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

HopDong* HopDong::create(const string& id, const string& ngayDK, const string& ngayHetHan, 
                            bool isActive, HoiVien* hv, GoiTap* gt, NhanVien* nv)
{
    return new HopDong(id, ngayDK, ngayHetHan, isActive, hv, gt, nv);
}

HopDong* HopDong::create(const string& ngayDK, const string& ngayHetHan, 
                            bool isActive, HoiVien* hv, GoiTap* gt, NhanVien* nv)
{
    string id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
    return new HopDong(id, ngayDK, ngayHetHan, isActive, hv, gt, nv);
}

string HopDong::read() {
    string result = id + ";" + ngayDK + ";" + ngayHetHan + ";" + (isActive ? "true" : "false") + ";" +
                    (hv ? hv->getID() : "NULL") + ";" +
                    (gt ? gt->getID() : "NULL") + ";" +
                    (nv ? nv->getID() : "NULL");
    return result;
}

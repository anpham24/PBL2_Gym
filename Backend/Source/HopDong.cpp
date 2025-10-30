#include "../Include/HopDong.h"
#include "../Include/HoiVien.h"
#include "../Include/GoiTap.h"
#include "../Include/NhanVien.h"
#include "../Include/IDGenerator.h"

HopDong::HopDong() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
}

HopDong::HopDong(HoiVien* hv, GoiTap* gt, NhanVien* nv, const string& ngayDK, const string& ngayHetHan)
    : hv(hv), gt(gt), nv(nv), ngayDK(ngayDK), ngayHetHan(ngayHetHan) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
}

HopDong::HopDong(const HopDong& other)
    : hv(other.hv), gt(other.gt), nv(other.nv), ngayDK(other.ngayDK), ngayHetHan(other.ngayHetHan) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
}

HopDong::~HopDong() {}

const string& HopDong::getID() const { return this->id; }
const HoiVien& HopDong::getHoiVien() const { return *hv; }
const GoiTap& HopDong::getGoiTap() const { return *gt; }
const NhanVien& HopDong::getNhanVien() const { return *nv; }
const string& HopDong::getNgayDK() const { return ngayDK; }
const string& HopDong::getNgayHetHan() const { return ngayHetHan; }

void HopDong::setHoiVien(HoiVien* hv) { this->hv = hv; }
void HopDong::setGoiTap(GoiTap* gt) { this->gt = gt; }
void HopDong::setNhanVien(NhanVien* nv) { this->nv = nv; }
void HopDong::setNgayDK(const string& ngay) { ngayDK = ngay; }
void HopDong::setNgayHetHan(const string& ngay) { ngayHetHan = ngay; }

HopDong* HopDong::create(HoiVien* hv, GoiTap* gt, NhanVien* nv,
                   const string& ngayDK, const string& ngayHetHan)
{
    return new HopDong(hv, gt, nv, ngayDK, ngayHetHan);
}

string HopDong::read() const {
    string result = hv->getID() + "," + gt->getID() + "," + nv->getID() + "," + ngayDK + "," + ngayHetHan;
    return result;
}

#include "HopDong.h"
#include "HoiVien.h"
#include "GoiTap.h"
#include "NhanVien.h"
#include "IDGenerator.h"
#include <ctime>
#include <iostream>

HopDong::HopDong() 
    : hv(nullptr), gt(nullptr), nv(nullptr), isActive(true) {}

int HopDong::getSoBuoiPTConLai() const { 
    return soBuoiPTConLai; 
}

void HopDong::setSoBuoiPTConLai(int soLuong) { 
    soBuoiPTConLai = soLuong; 
}

void HopDong::truBuoiPT(int soLuong) {
    if (soBuoiPTConLai >= soLuong) {
        soBuoiPTConLai -= soLuong;
    }
}

// ✅ UPDATE constructor
HopDong::HopDong(const string& id, const string& ngayDK, const string& ngayHetHan, 
                 bool isActive, int soBuoiPT, HoiVien* hv, GoiTap* gt, NhanVien* nv)
    : id(id), ngayDK(ngayDK), ngayHetHan(ngayHetHan), isActive(isActive),
      soBuoiPTConLai(soBuoiPT), // ✅ THÊM
      hv(nullptr), gt(nullptr), nv(nullptr)
{
    if (hv != nullptr) setHoiVien(hv);
    if (gt != nullptr) setGoiTap(gt);
    if (nv != nullptr) setNhanVien(nv);
}

HopDong::HopDong(const HopDong& other)
    : id(other.id), ngayDK(other.ngayDK), ngayHetHan(other.ngayHetHan), 
      isActive(other.isActive), hv(nullptr), gt(nullptr), nv(nullptr)
{
    if (other.hv != nullptr) setHoiVien(other.hv);
    if (other.gt != nullptr) setGoiTap(other.gt);
    if (other.nv != nullptr) setNhanVien(other.nv);
}

HopDong::~HopDong() {
    // ✅ Xóa quan hệ 2 chiều khi destroy
    setHoiVien(nullptr);
    setGoiTap(nullptr);
    setNhanVien(nullptr);
}

const string& HopDong::getID() const { return this->id; }

const HoiVien& HopDong::getHoiVien() const { 
    if (hv == nullptr) {
        static HoiVien nullHV; // Fallback để tránh crash
        return nullHV;
    }
    return *hv; 
}

const GoiTap& HopDong::getGoiTap() const { 
    if (gt == nullptr) {
        static GoiTap nullGT;
        return nullGT;
    }
    return *gt; 
}

const NhanVien& HopDong::getNhanVien() const { 
    if (nv == nullptr) {
        static NhanVien nullNV;
        return nullNV;
    }
    return *nv; 
}

const string& HopDong::getNgayDK() const { return ngayDK; }
const string& HopDong::getNgayHetHan() const { return ngayHetHan; }

bool HopDong::getIsActive() { 
    tm t = {};

    t.tm_mday = stoi(this->ngayHetHan.substr(0, 2));
    t.tm_mon  = stoi(this->ngayHetHan.substr(3, 2)) - 1;
    t.tm_year = stoi(this->ngayHetHan.substr(6, 4)) - 1900;

    time_t ngayHetHanEpoch = mktime(&t);
    time_t now = time(nullptr);

    if (ngayHetHanEpoch < now)
        this->isActive = false;

    return this->isActive;
}

void HopDong::setHoiVien(HoiVien* newHV) { 
    if (newHV == this->hv) return; // Tránh vòng lặp
    
    // Xóa khỏi HoiVien cũ
    if (this->hv != nullptr) {
        this->hv->removeHopDong(this);
    }
    
    // Cập nhật pointer
    this->hv = newHV;
    
    // Thêm vào HoiVien mới
    if (newHV != nullptr) {
        newHV->addHopDong(this);
    }
}

void HopDong::setGoiTap(GoiTap* newGT) { 
    if (this->gt == newGT) return;
    
    if (this->gt != nullptr) {
        this->gt->removeHopDong(this);
    }
    
    this->gt = newGT;
    
    if (newGT != nullptr) {
        newGT->addHopDong(this);
    }
}

void HopDong::setNhanVien(NhanVien* newNV) { 
    if (this->nv == newNV) return;
    
    if (this->nv != nullptr) {
        this->nv->removeHopDong(this);
    }
    
    this->nv = newNV;
    
    if (newNV != nullptr) {
        newNV->addHopDong(this);
    }
}

void HopDong::setNgayDK(const string& ngay) { ngayDK = ngay; }
void HopDong::setNgayHetHan(const string& ngay) { ngayHetHan = ngay; }
void HopDong::setIsActive(bool active) { isActive = active; }

HopDong* HopDong::create(const string& id, const string& ngayDK, const string& ngayHetHan, 
                        bool isActive, int soBuoiPT, HoiVien* hv, GoiTap* gt, NhanVien* nv)
{
    std::cout << "🔨 HopDong::create() - PT sessions: " << soBuoiPT << std::endl;
    return new HopDong(id, ngayDK, ngayHetHan, isActive, soBuoiPT, hv, gt, nv);
}

HopDong* HopDong::create(const string& ngayDK, const string& ngayHetHan, 
                        bool isActive, int soBuoiPT, HoiVien* hv, GoiTap* gt, NhanVien* nv)
{
    string id = IDGenerator::generateID(IDGenerator::Prefix_HopDong);
    return HopDong::create(id, ngayDK, ngayHetHan, isActive, soBuoiPT, hv, gt, nv);
}

string HopDong::read() {
    string result = id + ";" + 
                    ngayDK + ";" + 
                    ngayHetHan + ";" + 
                    to_string(isActive) + ";" +
                    to_string(soBuoiPTConLai) + ";" + // ✅ THÊM
                    (hv ? hv->getID() : "NULL") + ";" +
                    (gt ? gt->getID() : "NULL") + ";" +
                    (nv ? nv->getID() : "ADMIN");
    return result;
}
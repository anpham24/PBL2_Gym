#include "HoaDon.h"
#include "NhanVien.h"
#include "HoiVien.h"
#include "ChiTietHoaDon_HH.h"
#include "ChiTietHoaDon_GT.h"
#include "IDGenerator.h"

HoaDon::HoaDon() 
    : daThanhToan(false), giamGia(0.0), nhanVien(nullptr), hoiVien(nullptr) {}

HoaDon::HoaDon(const string& id, const string& ngayLap, const string& phuongThucTT, NhanVien* nv, HoiVien* hv) 
    : id(id), nhanVien(nv), hoiVien(hv), ngayLap(ngayLap), phuongThucTT(phuongThucTT), 
      daThanhToan(false), giamGia(0.0) {}

HoaDon::HoaDon(const HoaDon& other)
    : id(other.id), nhanVien(other.nhanVien), hoiVien(other.hoiVien), 
      ngayLap(other.ngayLap), phuongThucTT(other.phuongThucTT), 
      daThanhToan(other.daThanhToan), giamGia(other.giamGia) {}

HoaDon::~HoaDon() {}

const string& HoaDon::getID() const { return id; }
const NhanVien* HoaDon::getNhanVien() const { return nhanVien; }
const HoiVien* HoaDon::getHoiVien() const { return hoiVien; }
const string& HoaDon::getNgayLap() const { return ngayLap; }
const string& HoaDon::getPhuongThuc() const { return phuongThucTT; }
const MyVector<ChiTietHoaDon_HH*>& HoaDon::getDsChiTietHoaDon_HH() const { return dsChiTietHoaDon_HH; }
const MyVector<ChiTietHoaDon_GT*>& HoaDon::getDsChiTietHoaDon_GT() const { return dsChiTietHoaDon_GT; }
MyVector<ChiTietHoaDon_HH*>& HoaDon::getDsChiTietHoaDon_HH() { return dsChiTietHoaDon_HH; }
MyVector<ChiTietHoaDon_GT*>& HoaDon::getDsChiTietHoaDon_GT() { return dsChiTietHoaDon_GT; }

// ✅ THÊM GETTER/SETTER GIẢM GIÁ
double HoaDon::getGiamGia() const { return giamGia; }
void HoaDon::setGiamGia(double g) { giamGia = g; }

void HoaDon::setNhanVien(NhanVien* nv) { 
    if (nv == this->nhanVien) return;
    if (this->nhanVien != nullptr) {
        this->nhanVien->removeHoaDon(this);
    }
    this->nhanVien = nv;
    if (this->nhanVien != nullptr) {
        this->nhanVien->addHoaDon(this);
    }
}

void HoaDon::setHoiVien(HoiVien* hv) { 
    if (hv == this->hoiVien) return;
    if (this->hoiVien != nullptr) {
        this->hoiVien->removeHoaDon(this);
    }
    this->hoiVien = hv;
    if (this->hoiVien != nullptr) {
        this->hoiVien->addHoaDon(this);
    }
}

void HoaDon::setNgayLap(const string &d) { ngayLap = d; }
void HoaDon::setPhuongThucTT(const string &p) { phuongThucTT = p; }

void HoaDon::addChiTietHoaDon_HH(ChiTietHoaDon_HH* item) {
    dsChiTietHoaDon_HH.push_back(item);
}

void HoaDon::removeChiTietHoaDon_HH(ChiTietHoaDon_HH* item) {
    for (size_t i = 0; i < dsChiTietHoaDon_HH.size(); i++) {
        if (dsChiTietHoaDon_HH[i] == item) {
            dsChiTietHoaDon_HH.erase(i);
            return;
        }
    }
}

void HoaDon::addChiTietHoaDon_GT(ChiTietHoaDon_GT* item) {
    dsChiTietHoaDon_GT.push_back(item);
}

void HoaDon::removeChiTietHoaDon_GT(ChiTietHoaDon_GT* item) {
    for (size_t i = 0; i < dsChiTietHoaDon_GT.size(); i++) {
        if (dsChiTietHoaDon_GT[i] == item) {
            dsChiTietHoaDon_GT.erase(i);
            return;
        }
    }
}

// ✅ Tổng GỐC (chưa giảm giá)
double HoaDon::getTotal() const {
    double sum = 0.0;
    for (size_t i = 0; i < dsChiTietHoaDon_HH.size(); ++i) {
        sum += dsChiTietHoaDon_HH[i]->tinhTien();
    }
    for (size_t i = 0; i < dsChiTietHoaDon_GT.size(); ++i) {
        sum += dsChiTietHoaDon_GT[i]->tinhTien();
    }
    return sum;
}

// ✅ THÊM: Tổng SAU GIẢM GIÁ
double HoaDon::getFinalTotal() const {
    return getTotal() - giamGia;
}

size_t HoaDon::itemCount() const {
    return dsChiTietHoaDon_HH.size() + dsChiTietHoaDon_GT.size();
}

HoaDon* HoaDon::create(const string& id, const string& ngayLap, const string& phuongThucTT, NhanVien* nv, HoiVien* hv) {
    return new HoaDon(id, ngayLap, phuongThucTT, nv, hv);
}

HoaDon* HoaDon::create(const string& ngayLap, const string& phuongThucTT, 
                       NhanVien* nv, HoiVien* hv) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HoaDon);
    
    HoaDon* hd = new HoaDon(id, ngayLap, phuongThucTT);
    
    // ✅ GỌI setters ĐỂ TẠO QUAN HỆ 2 CHIỀU
    if (nv != nullptr) {
        hd->setNhanVien(nv);
    }
    
    if (hv != nullptr) {
        hd->setHoiVien(hv);
    }
    
    return hd;
}

string HoaDon::read() const {
    string result = id + ";" + 
                    ngayLap + ";" + 
                    phuongThucTT + ";" + 
                    to_string(giamGia) + ";" +
                    (nhanVien ? nhanVien->getID() : "ADMIN") + ";" + // ✅ Lưu cả NV
                    (hoiVien ? hoiVien->getID() : "NULL");           // ✅ Lưu cả HV
    return result;
}

void HoaDon::setDaThanhToan(bool status) {
    this->daThanhToan = status; 
}

bool HoaDon::getDaThanhToan() const {
    return this->daThanhToan; 
}
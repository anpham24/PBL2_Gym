#include "HoaDon.h"
#include "NhanVien.h"
#include "HoiVien.h"
#include "ChiTietHoaDon_HH.h"
#include "ChiTietHoaDon_GT.h"
#include "IDGenerator.h"

HoaDon::HoaDon() {}

HoaDon::HoaDon(const string& id, const string& ngayLap, const string& phuongThucTT, NhanVien* nv, HoiVien* hv) 
    : id(id), nhanVien(nv), hoiVien(hv), ngayLap(ngayLap), phuongThucTT(phuongThucTT) {}

HoaDon::HoaDon(const HoaDon& other)
    : id(other.id), nhanVien(other.nhanVien), hoiVien(other.hoiVien), 
      ngayLap(other.ngayLap), phuongThucTT(other.phuongThucTT) {}

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
    for (size_t i = 0; i < dsChiTietHoaDon_HH.size(); ++i) {
        if (dsChiTietHoaDon_HH.at(i) == item) {
            dsChiTietHoaDon_HH.erase(i);
            return;
        }
    }
}

void HoaDon::addChiTietHoaDon_GT(ChiTietHoaDon_GT* item) {
    dsChiTietHoaDon_GT.push_back(item);
}

void HoaDon::removeChiTietHoaDon_GT(ChiTietHoaDon_GT* item) {
    for (size_t i = 0; i < dsChiTietHoaDon_GT.size(); ++i) {
        if (dsChiTietHoaDon_GT.at(i) == item) {
            dsChiTietHoaDon_GT.erase(i);
            return;
        }
    }
}

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

size_t HoaDon::itemCount() const {
    return dsChiTietHoaDon_HH.size() + dsChiTietHoaDon_GT.size();
}

HoaDon* HoaDon::create(const string& id, const string& ngayLap, const string& phuongThucTT, NhanVien* nv, HoiVien* hv) {
    return new HoaDon(id, ngayLap, phuongThucTT, nv, hv);
}

HoaDon* HoaDon::create(const string& ngayLap, const string& phuongThucTT, NhanVien* nv, HoiVien* hv) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HoaDon);
    return new HoaDon(id, ngayLap, phuongThucTT, nv, hv);
}

string HoaDon::read() const {
    string result = id + "," + nhanVien->getID() + "," + hoiVien->getID() + "," + ngayLap + "," + phuongThucTT + ",";
    
    // Thêm danh sách chi tiết hàng hóa
    for (size_t i = 0; i < dsChiTietHoaDon_HH.size(); ++i) {
        result += dsChiTietHoaDon_HH.at(i)->read();
        if (i != dsChiTietHoaDon_HH.size() - 1 || dsChiTietHoaDon_GT.size() > 0) {
            result += ";"; // Ngăn cách các chi tiết bằng dấu chấm phẩy
        }
    }
    
    // Thêm danh sách chi tiết gói tập
    for (size_t i = 0; i < dsChiTietHoaDon_GT.size(); ++i) {
        result += dsChiTietHoaDon_GT.at(i)->read();
        if (i != dsChiTietHoaDon_GT.size() - 1) {
            result += ";"; // Ngăn cách các chi tiết bằng dấu chấm phẩy
        }
    }
    
    return result;
}

#include "../Include/HoaDon.h"
#include "../Include/NhanVien.h"
#include "../Include/HoiVien.h"
#include "../Include/ChiTietHoaDon_HH.h"
#include "../Include/ChiTietHoaDon_GT.h"
#include "../Include/IDGenerator.h"

HoaDon::HoaDon() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HoaDon);
}

HoaDon::HoaDon(NhanVien* nv, HoiVien* hv, const string& ngayLap, const string& phuongThucTT) 
    : nhanVien(nv), hoiVien(hv), ngayLap(ngayLap), phuongThucTT(phuongThucTT) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HoaDon);
}

HoaDon::HoaDon(const HoaDon& other)
    : nhanVien(other.nhanVien), hoiVien(other.hoiVien), 
      ngayLap(other.ngayLap), phuongThucTT(other.phuongThucTT), 
      itemsHH(other.itemsHH), itemsGT(other.itemsGT) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HoaDon);
}

HoaDon::~HoaDon() {}

const string& HoaDon::getID() const { return id; }
const NhanVien* HoaDon::getNhanVien() const { return nhanVien; }
const HoiVien* HoaDon::getHoiVien() const { return hoiVien; }
const string& HoaDon::getNgayLap() const { return ngayLap; }
const string& HoaDon::getPhuongThuc() const { return phuongThucTT; }
const MyVector<ChiTietHoaDon_HH*>& HoaDon::getItemsHH() const { return itemsHH; }
const MyVector<ChiTietHoaDon_GT*>& HoaDon::getItemsGT() const { return itemsGT; }

void HoaDon::setNhanVien(NhanVien* nv) { nhanVien = nv; }
void HoaDon::setHoiVien(HoiVien* hv) { hoiVien = hv; }
void HoaDon::setNgayLap(const string &d) { ngayLap = d; }
void HoaDon::setPhuongThucTT(const string &p) { phuongThucTT = p; }

void HoaDon::addItemHH(ChiTietHoaDon_HH* item) {
    itemsHH.push_back(item);
}

void HoaDon::removeItemHH(ChiTietHoaDon_HH* item) {
    for (size_t i = 0; i < itemsHH.size(); ++i) {
        if (itemsHH.at(i) == item) {
            itemsHH.erase(i);
            return;
        }
    }
}

void HoaDon::addItemGT(ChiTietHoaDon_GT* item) {
    itemsGT.push_back(item);
}

void HoaDon::removeItemGT(ChiTietHoaDon_GT* item) {
    for (size_t i = 0; i < itemsGT.size(); ++i) {
        if (itemsGT.at(i) == item) {
            itemsGT.erase(i);
            return;
        }
    }
}

double HoaDon::getTotal() const {
    double sum = 0.0;
    for (size_t i = 0; i < itemsHH.size(); ++i) {
        sum += itemsHH[i]->tinhTien();
    }
    for (size_t i = 0; i < itemsGT.size(); ++i) {
        sum += itemsGT[i]->tinhTien();
    }
    return sum;
}

size_t HoaDon::itemCount() const {
    return itemsHH.size() + itemsGT.size();
}

HoaDon* HoaDon::create(NhanVien* nv, HoiVien* hv, const string& ngayLap, const string& phuongThucTT) {
    return new HoaDon(nv, hv, ngayLap, phuongThucTT);
}

string HoaDon::read() const {
    string result = id + "," + nhanVien->getID() + "," + hoiVien->getID() + "," + ngayLap + "," + phuongThucTT + ",";
    // Thêm danh sách chi tiết hàng hóa
    for (size_t i = 0; i < itemsHH.size(); ++i) {
        result += itemsHH[i]->read();
        if (i != itemsHH.size() - 1 || itemsGT.size() > 0) {
            result += ";"; // Ngăn cách các chi tiết bằng dấu chấm phẩy
        }
    }
    // Thêm danh sách chi tiết gói tập
    for (size_t i = 0; i < itemsGT.size(); ++i) {
        result += itemsGT[i]->read();
        if (i != itemsGT.size() - 1) {
            result += ";"; // Ngăn cách các chi tiết bằng dấu chấm phẩy
        }
    }
    return result;
}

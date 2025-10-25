#include "../Include/HoaDon.h"
#include "../Include/IDGenerator.h"
#define HD_ID 7

HoaDon::HoaDon() {
    this->id = IDGenerator::generateID(HD_ID);
}

HoaDon::HoaDon(const string& maHV, const string& maNV, const string& ngayLap, const string& phuongThucTT) 
    : maHV(maHV), maNV(maNV), ngayLap(ngayLap), phuongThucTT(phuongThucTT) {
    this->id = IDGenerator::generateID(HD_ID);
}

HoaDon::HoaDon(const HoaDon& other)
    : id(other.id), maHV(other.maHV), maNV(other.maNV), 
      ngayLap(other.ngayLap), phuongThucTT(other.phuongThucTT), 
      items(other.items) {}

HoaDon::~HoaDon() {}

const string& HoaDon::getID() const { return id; }
const string& HoaDon::getMaNV() const { return maNV; }
const string& HoaDon::getNgayLap() const { return ngayLap; }
const string& HoaDon::getPhuongThuc() const { return phuongThucTT; }
const string& HoaDon::getMaHV() const { return maHV; }
const MyVector<ChiTietHD>& HoaDon::getItems() const { return items; }

void HoaDon::setMaHV(const string &m) { maHV = m; }
void HoaDon::setMaNV(const string &m) { maNV = m; }
void HoaDon::setNgayLap(const string &d) { ngayLap = d; }
void HoaDon::setPhuongThucTT(const string &p) { phuongThucTT = p; }

void HoaDon::addItem(const ChiTietHD& ct) {
    items.push_back(ct);
}

bool HoaDon::removeItemByMaSP(const string &maSP) {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].getMaSP() == maSP) {
            items.erase(items.begin() + i); 
            return true;
        }
    }
    return false; 
}

double HoaDon::getTotal() const {
    double sum = 0.0;
    for (size_t i = 0; i < items.size(); ++i) {
        sum += items[i].tinhTien();
    }
    return sum;
}

size_t HoaDon::itemCount() const {
    return items.size();
}
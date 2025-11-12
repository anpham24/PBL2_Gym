#include "GoiTap.h"
#include "MonTap.h"
#include "ChiTietHoaDon_GT.h"
#include "IDGenerator.h"

GoiTap::GoiTap() {}

GoiTap::GoiTap(const string& id, const string& tenGoi, int thoiGian, int soBuoiPT, double gia, bool isActive)
    : id(id), tenGoi(tenGoi), thoiGian(thoiGian), soBuoiPT(soBuoiPT), gia(gia), isActive(isActive) {}

GoiTap::GoiTap(const GoiTap& other) 
    : id(other.id), tenGoi(other.tenGoi), thoiGian(other.thoiGian), soBuoiPT(other.soBuoiPT), gia(other.gia), isActive(other.isActive) {}

GoiTap::~GoiTap() {}

const string& GoiTap::getID() const { return this->id; }
const string& GoiTap::getTenGoi() const { return this->tenGoi; }
int GoiTap::getThoiGian() const { return this->thoiGian; }
int GoiTap::getSoBuoiPT() const { return this->soBuoiPT; }
double GoiTap::getGia() const { return this->gia; }
bool GoiTap::getIsActive() const { return this->isActive; }

void GoiTap::setTenGoi(const string& t) { this->tenGoi = t; }
void GoiTap::setThoiGian(int tg) { this->thoiGian = tg; }
void GoiTap::setSoBuoiPT(int sb) { this->soBuoiPT = sb; }
void GoiTap::setGia(double g) { this->gia = g; }
void GoiTap::setIsActive(bool active) { this->isActive = active; }

void GoiTap::addMonTap(MonTap* mt) {
    if (mt == nullptr) return;
    this->dsMonTap.push_back(mt);
    mt->addGoiTap(this);
}

void GoiTap::removeMonTap(MonTap* mt) {
    for (size_t i = 0; i < dsMonTap.size(); i++) {
        if (dsMonTap[i] == mt) {
            dsMonTap.erase(i);
            return;
        }
    }
}

const MyVector<MonTap*>& GoiTap::getDsMonTap() const {
    return this->dsMonTap;
}

MyVector<MonTap*>& GoiTap::getDsMonTap() {
    return this->dsMonTap;
}

void GoiTap::addHopDong(HopDong* hd) {
    this->dsHopDong.push_back(hd);
}

void GoiTap::removeHopDong(HopDong* hd) {
    for (size_t i = 0; i < dsHopDong.size(); i++) {
        if (dsHopDong[i] == hd) {
            dsHopDong.erase(i);
            return;
        }
    }
}

const MyVector<HopDong*>& GoiTap::getDsHopDong() const {
    return this->dsHopDong;
}

MyVector<HopDong*>& GoiTap::getDsHopDong() {
    return this->dsHopDong;
}

void GoiTap::addChiTietHoaDon_GT(ChiTietHoaDon_GT* ct) {
    this->dsChiTietHoaDon_GT.push_back(ct);
}

void GoiTap::removeChiTietHoaDon_GT(ChiTietHoaDon_GT* ct) {
    for (size_t i = 0; i < dsChiTietHoaDon_GT.size(); i++) {
        if (dsChiTietHoaDon_GT[i] == ct) {
            dsChiTietHoaDon_GT.erase(i);
            return;
        }
    }
}

const MyVector<ChiTietHoaDon_GT*>& GoiTap::getDsChiTietHoaDon_GT() const {
    return this->dsChiTietHoaDon_GT;
}

MyVector<ChiTietHoaDon_GT*>& GoiTap::getDsChiTietHoaDon_GT() {
    return this->dsChiTietHoaDon_GT;
}

GoiTap* GoiTap::create(const string& id, const string& tenGoi, int thoiGian, int soBuoiPT, double gia, bool isActive) {
    return new GoiTap(id, tenGoi, thoiGian, soBuoiPT, gia, isActive);
}

GoiTap* GoiTap::create(const string& tenGoi, int thoiGian, int soBuoiPT, double gia, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
    return new GoiTap(id, tenGoi, thoiGian, soBuoiPT, gia, isActive);
}

string GoiTap::read() const {
    string result = id + ";" + tenGoi + ";" + to_string(thoiGian) + ";" + to_string(soBuoiPT) + ";" + to_string(gia) + ";" + (isActive ? "true" : "false");
    return result;
}
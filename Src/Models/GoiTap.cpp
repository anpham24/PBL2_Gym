#include "GoiTap.h"
#include "MonTap.h"
#include "ChiTietHoaDon_GT.h"
#include "IDGenerator.h"

GoiTap::GoiTap() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
}

GoiTap::GoiTap(const string& tenGoi, int thoiGian, double gia, bool isActive)
    : tenGoi(tenGoi), thoiGian(thoiGian), gia(gia), isActive(isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
}

GoiTap::GoiTap(const GoiTap& other) 
    : tenGoi(other.tenGoi), thoiGian(other.thoiGian), gia(other.gia), isActive(other.isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_GoiTap);
}

GoiTap::~GoiTap() {}

const string& GoiTap::getID() const { return this->id; }
const string& GoiTap::getTenGoi() const { return this->tenGoi; }
int GoiTap::getThoiGian() const { return this->thoiGian; }
double GoiTap::getGia() const { return this->gia; }
bool GoiTap::getIsActive() const { return this->isActive; }

void GoiTap::setTenGoi(const string& t) { this->tenGoi = t; }
void GoiTap::setThoiGian(int tg) { this->thoiGian = tg; }
void GoiTap::setGia(double g) { this->gia = g; }
void GoiTap::setIsActive(bool active) { this->isActive = active; }

void GoiTap::addMonTap(MonTap* mt) {
    this->dsMonTap.push_back(mt);
}

void GoiTap::removeMonTap(MonTap* mt) {
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap.at(i) == mt) {
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
    for (size_t i = 0; i < dsHopDong.size(); ++i) {
        if (dsHopDong.at(i) == hd) {
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
    for (size_t i = 0; i < dsChiTietHoaDon_GT.size(); ++i) {
        if (dsChiTietHoaDon_GT.at(i) == ct) {
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

GoiTap* GoiTap::create(const string& tenGoi, int thoiGian, double gia, bool isActive) {
    return new GoiTap(tenGoi, thoiGian, gia, isActive);
}


string GoiTap::read() const {
    string result = id + "," + tenGoi + "," + to_string(thoiGian) + "," + to_string(gia) + "," + to_string(isActive);
    // Thêm danh sách mã môn
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        result += dsMonTap[i]->getID();
        if (i != dsMonTap.size() - 1) {
            result += ";"; // Ngăn cách các mã môn bằng dấu chấm phẩy
        }
    }
    return result;
}
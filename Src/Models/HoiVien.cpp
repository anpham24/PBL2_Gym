#include "HoiVien.h"
#include "HopDong.h"
#include "HoaDon.h"
#include "IDGenerator.h"
#include "Config.h"

HoiVien::HoiVien() {}

HoiVien::HoiVien(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, int point, int soBuoiPT, bool isActive)
    : Person(id, hoTen, sdt, gioiTinh, ngaySinh), point(point), soBuoiPT(soBuoiPT), isActive(isActive) {}

HoiVien::HoiVien(const HoiVien& other) 
    : Person(other.id, other.hoTen, other.sdt, other.gioiTinh, other.ngaySinh), point(other.point), soBuoiPT(other.soBuoiPT), isActive(other.isActive) {}

HoiVien::~HoiVien() {}

// Getters
int HoiVien::getPoint() const { return this->point; }
int HoiVien::getSoBuoiPT() const { return this->soBuoiPT; }
bool HoiVien::getIsActive() const { return this->isActive; }
Config::Rank HoiVien::getRankEnum() const {
    if (point >= Config::RANK_MIN_POINTS.at(Config::Rank::PLATINUM)) {
        return Config::Rank::PLATINUM;
    } else if (point >= Config::RANK_MIN_POINTS.at(Config::Rank::GOLD)) {
        return Config::Rank::GOLD;
    } else if (point >= Config::RANK_MIN_POINTS.at(Config::Rank::SILVER)) {
        return Config::Rank::SILVER;
    } else {
        return Config::Rank::BRONZE;
    }
}

// ✅ Trả về string
string HoiVien::getRank() const {
    Config::Rank rank = getRankEnum();
    
    switch (rank) {
        case Config::Rank::PLATINUM: return "Platinum";
        case Config::Rank::GOLD:     return "Gold";
        case Config::Rank::SILVER:   return "Silver";
        case Config::Rank::BRONZE:
        default:                     return "Bronze";
    }
}

// ✅ Lấy % giảm giá
double HoiVien::getDiscountRate() const {
    return Config::RANK_DISCOUNT.at(getRankEnum());
}

// Setters
void HoiVien::setPoint(int point) { this->point = point; }
void HoiVien::setSoBuoiPT(int soBuoiPT) { this->soBuoiPT = soBuoiPT; }
void HoiVien::setIsActive(bool isActive) { this->isActive = isActive; } 

void HoiVien::addHoaDon(HoaDon* hoaDon) {
    dsHoaDon.push_back(hoaDon);
}

void HoiVien::removeHoaDon(HoaDon* hoaDon) {
    for (size_t i = 0; i < dsHoaDon.size(); i++) {
        if (dsHoaDon[i] == hoaDon) {
            dsHoaDon.erase(i);
            break;
        }
    }
}

const MyVector<HoaDon*>& HoiVien::getDsHoaDon() const {
    return dsHoaDon;
}

MyVector<HoaDon*>& HoiVien::getDsHoaDon() {
    return dsHoaDon;
}

void HoiVien::addHopDong(HopDong* hopDong) {
    dsHopDong.push_back(hopDong);
}

void HoiVien::removeHopDong(HopDong* hopDong) {
    for (size_t i = 0; i < dsHopDong.size(); i++) {
        if (dsHopDong[i] == hopDong) {
            dsHopDong.erase(i);
            break;
        }
    }
}

const MyVector<HopDong*>& HoiVien::getDsHopDong() const {
    return dsHopDong;
}

MyVector<HopDong*>& HoiVien::getDsHopDong() {
    return dsHopDong;
}

void HoiVien::addLogTapPT(LogTapPT* logTapPT) {
    dsLogTapPT.push_back(logTapPT);
}

void HoiVien::removeLogTapPT(LogTapPT* logTapPT) {
    for (size_t i = 0; i < dsLogTapPT.size(); i++) {
        if (dsLogTapPT[i] == logTapPT) {
            dsLogTapPT.erase(i);
            break;
        }
    }
}

const MyVector<LogTapPT*>& HoiVien::getDsLogTapPT() const {
    return dsLogTapPT;
}

MyVector<LogTapPT*>& HoiVien::getDsLogTapPT() {
    return dsLogTapPT;
}

HoiVien* HoiVien::create(const string& id, const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, int point, int soBuoiPT, bool isActive) {
    return new HoiVien(id, hoTen, sdt, gioiTinh, ngaySinh, point, soBuoiPT, isActive);
}

HoiVien* HoiVien::create(const string& hoTen, const string& sdt, const string& gioiTinh, const string& ngaySinh, int point, int soBuoiPT, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HoiVien);
    return new HoiVien(id, hoTen, sdt, gioiTinh, ngaySinh, point, soBuoiPT, isActive);
}

string HoiVien::read() const{
    string result = id + ";" + hoTen + ";" + sdt + ";" + gioiTinh + ";" + ngaySinh + ";" + to_string(point) + ";" + to_string(soBuoiPT) + ";" + (isActive ? "true" : "false");
    return result;
}

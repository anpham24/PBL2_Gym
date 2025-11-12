#include "Models/HangHoa.h"
#include "Models/ChiTietHoaDon_HH.h"
#include "Utils/IDGenerator.h"

HangHoa::HangHoa() {}

HangHoa::HangHoa(const string& id, const string& tenHH, const string& loaiHH, double gia, int soLuongCon, bool isActive) 
    : id(id), tenHH(tenHH), loaiHH(loaiHH), gia(gia), soLuongCon(soLuongCon), isActive(isActive) {}

HangHoa::HangHoa(const HangHoa& other) 
    : id(other.id), tenHH(other.tenHH), loaiHH(other.loaiHH), gia(other.gia), soLuongCon(other.soLuongCon), isActive(other.isActive) {}

HangHoa::~HangHoa() {}

const string& HangHoa::getID() const { return this->id; }
const string& HangHoa::getTenHH() const { return this->tenHH; }
const string& HangHoa::getLoaiHH() const { return this->loaiHH; }
double HangHoa::getGia() const { return this->gia; }
int HangHoa::getSoLuongCon() const { return this->soLuongCon; }
bool HangHoa::getIsActive() const { return this->isActive; }

void HangHoa::setTenHH(const string& tenHH) { this->tenHH = tenHH; }
void HangHoa::setLoaiHH(const string& loaiHH) { this->loaiHH = loaiHH; }
void HangHoa::setGia(double gia) { this->gia = gia; }
void HangHoa::setSoLuongCon(int soLuongCon) { this->soLuongCon = soLuongCon; }
void HangHoa::setIsActive(bool isActive) { this->isActive = isActive; }

void HangHoa::addChiTietHoaDon_HH(ChiTietHoaDon_HH* ct) {
    this->dsChiTietHoaDon_HH.push_back(ct);
}

void HangHoa::removeChiTietHoaDon_HH(ChiTietHoaDon_HH* ct) {
    for (size_t i = 0; i < dsChiTietHoaDon_HH.size(); i++) {
        if (dsChiTietHoaDon_HH[i] == ct) {
            dsChiTietHoaDon_HH.erase(i);
            return;
        }
    }
}

const MyVector<ChiTietHoaDon_HH*>& HangHoa::getDsChiTietHoaDon_HH() const {
    return this->dsChiTietHoaDon_HH;
}

MyVector<ChiTietHoaDon_HH*>& HangHoa::getDsChiTietHoaDon_HH() {
    return this->dsChiTietHoaDon_HH;
}

HangHoa* HangHoa::create(const string& id, const string& tenHH, const string& loaiHH, double gia, int soLuongCon, bool isActive) {
    return new HangHoa(id, tenHH, loaiHH, gia, soLuongCon, isActive);
}

HangHoa* HangHoa::create(const string& tenHH, const string& loaiHH, double gia, int soLuongCon, bool isActive) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
    return new HangHoa(id, tenHH, loaiHH, gia, soLuongCon, isActive);
}

string HangHoa::read() const {
    return id + ";" + tenHH + ";" + loaiHH + ";" + to_string(gia) + ";" + to_string(soLuongCon) + ";" + (isActive ? "true" : "false");
}
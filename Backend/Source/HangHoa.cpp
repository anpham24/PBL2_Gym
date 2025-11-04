#include "../Include/HangHoa.h"
#include "../Include/ChiTietHoaDon_HH.h"
#include "../Include/IDGenerator.h"

HangHoa::HangHoa() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
}

HangHoa::HangHoa(const string& tenHH, double gia, int soLuongCon, bool isActive) : tenHH(tenHH), gia(gia), soLuongCon(soLuongCon), isActive(isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
}

HangHoa::HangHoa(const HangHoa& other) 
    : tenHH(other.tenHH), gia(other.gia), soLuongCon(other.soLuongCon), isActive(other.isActive) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
}

HangHoa::~HangHoa() {}

const string& HangHoa::getID() const { return this->id; }
const string& HangHoa::getTenHH() const { return this->tenHH; }
double HangHoa::getGia() const { return this->gia; }
int HangHoa::getSoLuongCon() const { return this->soLuongCon; }
bool HangHoa::getIsActive() const { return this->isActive; }

void HangHoa::setTenHH(const string& tenHH) { this->tenHH = tenHH; }
void HangHoa::setGia(double gia) { this->gia = gia; }
void HangHoa::setSoLuongCon(int soLuongCon) { this->soLuongCon = soLuongCon; }
void HangHoa::setIsActive(bool isActive) { this->isActive = isActive; }

void HangHoa::addChiTietHoaDon_HH(ChiTietHoaDon_HH* ct) {
    this->dsChiTietHoaDon_HH.push_back(ct);
}

void HangHoa::removeChiTietHoaDon_HH(ChiTietHoaDon_HH* ct) {
    for (size_t i = 0; i < dsChiTietHoaDon_HH.size(); ++i) {
        if (dsChiTietHoaDon_HH.at(i) == ct) {
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

HangHoa* HangHoa::create(const string& tenHH, double gia, int soLuongCon, bool isActive) {
    return new HangHoa(tenHH, gia, soLuongCon, isActive);
}

string HangHoa::read() const {
    return id + "," + tenHH + "," + to_string(gia) + "," + to_string(soLuongCon) + "," + to_string(isActive);
}
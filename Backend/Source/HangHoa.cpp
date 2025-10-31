#include "../Include/HangHoa.h"
#include "../Include/IDGenerator.h"

HangHoa::HangHoa() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
}

HangHoa::HangHoa(const string& tenHH, double gia, int soLuongCon) : tenHH(tenHH), gia(gia), soLuongCon(soLuongCon) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
}

HangHoa::HangHoa(const HangHoa& other) 
    : tenHH(other.tenHH), gia(other.gia), soLuongCon(other.soLuongCon) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_HangHoa);
}

HangHoa::~HangHoa() {}

const string& HangHoa::getID() const { return this->id; }
const string& HangHoa::getTenHH() const { return this->tenHH; }
double HangHoa::getGia() const { return this->gia; }
int HangHoa::getSoLuongCon() const { return this->soLuongCon; }

void HangHoa::setTenHH(const string& tenHH) { this->tenHH = tenHH; }
void HangHoa::setGia(double gia) { this->gia = gia; }
void HangHoa::setSoLuongCon(int soLuongCon) { this->soLuongCon = soLuongCon; }

HangHoa* HangHoa::create(const string& tenHH, double gia, int soLuongCon) {
    return new HangHoa(tenHH, gia, soLuongCon);
}

string HangHoa::read() const {
    return id + "," + tenHH + "," + to_string(gia) + "," + to_string(soLuongCon);
}
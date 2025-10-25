#include "../Include/HangHoa.h"
#include "../Include/IDGenerator.h"
#define HH_ID 6

HangHoa::HangHoa() {
    this->id = IDGenerator::generateID(HH_ID);
}

HangHoa::HangHoa(const string& tenHH, double gia) : tenHH(tenHH), gia(gia) {
    this->id = IDGenerator::generateID(HH_ID);
}

HangHoa::~HangHoa() {}

const string& HangHoa::getID() const { return this->id; }
const string& HangHoa::getTenHH() const { return this->tenHH; }
double HangHoa::getGia() const { return this->gia; }
 
void HangHoa::setTenHH(const string& tenHH) { this->tenHH = tenHH; }
void HangHoa::setGia(double gia) { this->gia = gia; } 

HangHoa HangHoa::create(const string& tenHH, double gia) {
    return HangHoa(tenHH, gia);
}

void HangHoa::update(const string& tenHH, double gia) {
    this->tenHH = tenHH;
    this->gia = gia;
}

string HangHoa::read() const {
    return id + "," + tenHH + "," + to_string(gia);
}
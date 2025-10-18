#include "../Include/HangHoa.h"
#include "../Include/IDGenerator.h"
#include <sstream>

HangHoa::HangHoa() {
    this->id = IDGenerator::generateID(60);
}

HangHoa::HangHoa(const string& tenHH, double gia) : tenHH(tenHH), gia(gia) {
    this->id = IDGenerator::generateID(60);
}

HangHoa::~HangHoa() {

}

string HangHoa::getID() const { return this->id; }
string HangHoa::getTenHH() const { return this->tenHH; }
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
    ostringstream oss;
    oss << id << " | " << tenHH << " | " << gia;
    return oss.str();
}
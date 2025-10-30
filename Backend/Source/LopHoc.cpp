#include "../Include/LopHoc.h"
#include "../Include/HLV.h"
#include "../Include/MonTap.h"
#include "../Include/IDGenerator.h"
#define LopHoc_ID 3 // Dùng chung prefix với MonTap

LopHoc::LopHoc() {
    this->id = IDGenerator::generateID(LopHoc_ID);
}

LopHoc::LopHoc(const string& tenLop, const string& lichTap, int thoiLuong, MonTap* monTap, HLV* hlv)
    : tenLop(tenLop), lichTap(lichTap), thoiLuong(thoiLuong), monTap(monTap), hlv(hlv) {
    this->id = IDGenerator::generateID(LopHoc_ID);
}

LopHoc::LopHoc(const LopHoc& other)
    : tenLop(other.tenLop), lichTap(other.lichTap), thoiLuong(other.thoiLuong),
      monTap(other.monTap), hlv(other.hlv) {
    this->id = IDGenerator::generateID(LopHoc_ID);
}

LopHoc::~LopHoc() {}

string LopHoc::getID() const { return this->id; }
string LopHoc::getTenLop() const { return this->tenLop; }
string LopHoc::getLichTap() const { return this->lichTap; }
int LopHoc::getThoiLuong() const { return this->thoiLuong; }
MonTap* LopHoc::getMonTap() const { return this->monTap; }
HLV* LopHoc::getHLV() const { return this->hlv; }

void LopHoc::setTenLop(const string& t) { this->tenLop = t; }
void LopHoc::setLichTap(const string& l) { this->lichTap = l; }
void LopHoc::setThoiLuong(int tl) { this->thoiLuong = tl; }
void LopHoc::setMonTap(MonTap* mt) { this->monTap = mt; }

void LopHoc::setHLV(HLV* h) { 
    if (this->hlv != nullptr) {
        this->hlv->removeLopHoc(this);
    }
    this->hlv = h;
    if (h != nullptr) {
        h->addLopHoc(this);
    }
}

LopHoc* LopHoc::create(const string& tenLop, const string& lichTap, const string& thoiLuongStr, int thoiLuong, MonTap* monTap, HLV* hlv) {
    return new LopHoc(tenLop, lichTap, thoiLuong, monTap, hlv);
}

string LopHoc::read() const {
    string result = id + "," + tenLop + "," + lichTap + "," + to_string(thoiLuong);
    if (monTap != nullptr) {
        result += "," + monTap->getID();
    } else {
        result += ",NULL";
    }
    if (hlv != nullptr) {
        result += "," + hlv->getID();
    } else {
        result += ",NULL";
    }
    return result;
}


#include "LopHoc.h"
#include "HLV.h"
#include "MonTap.h"
#include "IDGenerator.h"

LopHoc::LopHoc() {}

LopHoc::LopHoc(const string& id, const string& tenLop, const string& lichTap, int thoiLuong, MonTap* monTap, HLV* hlv)
    : id(id), tenLop(tenLop), lichTap(lichTap), thoiLuong(thoiLuong), monTap(monTap), hlv(hlv) {}

LopHoc::LopHoc(const LopHoc& other)
    : id(other.id), tenLop(other.tenLop), lichTap(other.lichTap), thoiLuong(other.thoiLuong),
      monTap(other.monTap), hlv(other.hlv) {}

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

void LopHoc::setMonTap(MonTap* mt) { 
    if (this->monTap != nullptr) {
        this->monTap->removeLopHoc(this);
    }
    this->monTap = mt;
    if (mt != nullptr) {
        mt->addLopHoc(this);
    }
}

void LopHoc::setHLV(HLV* h) { 
    if (this->hlv != nullptr) {
        this->hlv->removeLopHoc(this);
    }
    this->hlv = h;
    if (h != nullptr) {
        h->addLopHoc(this);
    }
}

LopHoc* LopHoc::create(const string& id, const string& tenLop, const string& lichTap, int thoiLuong, MonTap* monTap, HLV* hlv) {
    return new LopHoc(id, tenLop, lichTap, thoiLuong, monTap, hlv);
}

LopHoc* LopHoc::create(const string& tenLop, const string& lichTap, int thoiLuong, MonTap* monTap, HLV* hlv) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_MonTap);
    return new LopHoc(id, tenLop, lichTap, thoiLuong, monTap, hlv);
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


#include "../Include/MonTap.h"
#include "../Include/LopHoc.h"
#include "../Include/GoiTap.h"
#include "../../Utils/Include/IDGenerator.h"

MonTap::MonTap() {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_MonTap);
}

MonTap::MonTap(const string& tenMon, const string& lichTap, const string& maHLV)
    : tenMon(tenMon), lichTap(lichTap), maHLV(maHLV) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_MonTap);
}

MonTap::MonTap(const MonTap& other) 
    : tenMon(other.tenMon), lichTap(other.lichTap), maHLV(other.maHLV) {
    this->id = IDGenerator::generateID(IDGenerator::Prefix_MonTap);
}

MonTap::~MonTap() {}

const string& MonTap::getID() const { return this->id; }
const string& MonTap::getTenMon() const { return this->tenMon; }
const string& MonTap::getLichTap() const { return this->lichTap; }
const string& MonTap::getMaHLV() const { return this->maHLV; }

void MonTap::setTenMon(const string& t) { this->tenMon = t; }
void MonTap::setLichTap(const string& l) { this->lichTap = l; }
void MonTap::setMaHLV(const string& m) { this->maHLV = m; }

void MonTap::addLopHoc(LopHoc* lh) {
    dsLopHoc.push_back(lh);
}

void MonTap::removeLopHoc(LopHoc* lh) {
    for (int i = 0; i < dsLopHoc.size(); i++) {
        if (dsLopHoc[i] == lh) {
            dsLopHoc.erase(i);
            break;
        }
    }
}

const MyVector<LopHoc*>& MonTap::getDsLopHoc() const {
    return dsLopHoc;
}

MyVector<LopHoc*>& MonTap::getDsLopHoc() {
    return dsLopHoc;
}

void MonTap::addGoiTap(GoiTap* gt) {
    dsGoiTap.push_back(gt);
}

void MonTap::removeGoiTap(GoiTap* gt) {
    for (int i = 0; i < dsGoiTap.size(); i++) {
        if (dsGoiTap[i] == gt) {
            dsGoiTap.erase(i);
            break;
        }
    }
}

const MyVector<GoiTap*>& MonTap::getDsGoiTap() const {
    return dsGoiTap;
}

MyVector<GoiTap*>& MonTap::getDsGoiTap() {
    return dsGoiTap;
}

MonTap* MonTap::create(const string& tenMon, const string& lichTap, const string& maHLV) {
    return new MonTap(tenMon, lichTap, maHLV);
}

string MonTap::read() const {
    string result = id + "," + tenMon + "," + lichTap + "," + maHLV;
    return result;
}

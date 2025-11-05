#include "MonTap.h"
#include "LopHoc.h"
#include "GoiTap.h"
#include "IDGenerator.h"

MonTap::MonTap() {}

MonTap::MonTap(const string& id, const string& tenMon)
    : id(id), tenMon(tenMon){}

MonTap::MonTap(const MonTap& other) 
    : id(other.id), tenMon(other.tenMon){}

MonTap::~MonTap() {}

const string& MonTap::getID() const { return this->id; }
const string& MonTap::getTenMon() const { return this->tenMon; }

void MonTap::setTenMon(const string& t) { this->tenMon = t; }

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

MonTap* MonTap::create(const string& id, const string& tenMon) {
    return new MonTap(id, tenMon);
}

MonTap* MonTap::create(const string& tenMon) {
    string id = IDGenerator::generateID(IDGenerator::Prefix_MonTap);
    return new MonTap(id, tenMon);
}

string MonTap::read() const {
    string result = id + "," + tenMon;
    return result;
}

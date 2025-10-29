#include "../Include/MonTap.h"
#include "../Include/IDGenerator.h"

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

MonTap* MonTap::create(const string& tenMon, const string& lichTap, const string& maHLV) {
    return new MonTap(tenMon, lichTap, maHLV);
}

string MonTap::read() const {
    string result = id + "," + tenMon + "," + lichTap + "," + maHLV;
    return result;
}

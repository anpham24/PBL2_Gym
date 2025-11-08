#include "LogTapPT.h"
#include "HoiVien.h"
#include "HLV.h"
#include "IDGenerator.h"

LogTapPT::LogTapPT() {}

LogTapPT::LogTapPT(const string& id, const string& ngayGio, HoiVien* hv, HLV* hlv)
    : id(id), ngayGio(ngayGio), hv(hv), hlv(hlv) {}

LogTapPT::LogTapPT(const LogTapPT& other)
    : id(other.id), ngayGio(other.ngayGio), hv(other.hv), hlv(other.hlv) {}

LogTapPT::~LogTapPT() {}

const string& LogTapPT::getID() const { return this->id; }
const string& LogTapPT::getNgayGio() const { return this->ngayGio; }
const HoiVien& LogTapPT::getHoiVien() const { return *(this->hv); }
const HLV& LogTapPT::getHLV() const { return *(this->hlv); }

void LogTapPT::setNgayGio(const string& ngayGio) { this->ngayGio = ngayGio; }
void LogTapPT::setHoiVien(HoiVien* hv) { 
    if (hv == this->hv) return;
    if (this->hv != nullptr) {
        this->hv->removeLogTapPT(this);
    }
    this->hv = hv;
    if (hv != nullptr) {
        hv->addLogTapPT(this);
    }
}
void LogTapPT::setHLV(HLV* hlv) { 
    if (hlv == this->hlv) return;
    if (this->hlv != nullptr) {
        this->hlv->removeLogTapPT(this);
    }
    this->hlv = hlv;
    if (hlv != nullptr) {
        hlv->addLogTapPT(this);
    }
}

LogTapPT* LogTapPT::create(const string& id, const string& ngayGio, HoiVien* hv, HLV* hlv) {
    return new LogTapPT(id, ngayGio, hv, hlv);
}

LogTapPT* LogTapPT::create(const string& ngayGio, HoiVien* hv, HLV* hlv) {
    hv->setSoBuoiPT(hv->getSoBuoiPT() - 1);
    string id = IDGenerator::generateID(IDGenerator::Prefix_LogTapPT);
    return new LogTapPT(id, ngayGio, hv, hlv);
}

string LogTapPT::read() const {
    string result = id + ";" + ngayGio + ";" + (hv ? hv->getID() : "NULL") + ";" + (hlv ? hlv->getID() : "NULL");
    return result;
}
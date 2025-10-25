#include "../Include/ThamGia.h"

ThamGia::ThamGia() {}

ThamGia::ThamGia(const string& maHV, const string& maGoi,
                 const string& ngayDK, const string& ngayHetHan)
    : maHV(maHV), maGoi(maGoi), ngayDK(ngayDK), ngayHetHan(ngayHetHan) {}

const string& ThamGia::getMaHV() const { return maHV; }
const string& ThamGia::getMaGoi() const { return maGoi; }
const string& ThamGia::getNgayDK() const { return ngayDK; }
const string& ThamGia::getNgayHetHan() const { return ngayHetHan; }

void ThamGia::setMaHV(const string& hv) { maHV = hv; }
void ThamGia::setMaGoi(const string& goi) { maGoi = goi; }
void ThamGia::setNgayDK(const string& ngay) { ngayDK = ngay; }
void ThamGia::setNgayHetHan(const string& ngay) { ngayHetHan = ngay; }

ThamGia ThamGia::create(const string& maHV, const string& maGoi,
                         const string& ngayDK, const string& ngayHetHan)
{
    return ThamGia(maHV, maGoi, ngayDK, ngayHetHan);
}

string ThamGia::read() const {
    string result = maHV + "," + maGoi + "," + ngayDK + "," + ngayHetHan;
    return result;
}

void ThamGia::update(const string& newMaHV, const string& newMaGoi,
                     const string& newNgayDK, const string& newNgayHetHan)
{
    this->maHV = newMaHV;
    this->maGoi = newMaGoi;
    this->ngayDK = newNgayDK;
    this->ngayHetHan = newNgayHetHan;
}
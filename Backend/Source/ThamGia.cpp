#include "../Include/ThamGia.h"

ThamGia::ThamGia() {
    
}

ThamGia::ThamGia(const string& maHV, const string& maGoi,
                 const string& ngayDK, const string& ngayHetHan)
    : maHV(maHV), maGoi(maGoi), ngayDK(ngayDK), ngayHetHan(ngayHetHan) {}

string ThamGia::getMaHV() const { return maHV; }
string ThamGia::getMaGoi() const { return maGoi; }
string ThamGia::getNgayDK() const { return ngayDK; }
string ThamGia::getNgayHetHan() const { return ngayHetHan; }

void ThamGia::setMaHV(const string& hv) { maHV = hv; }
void ThamGia::setMaGoi(const string& goi) { maGoi = goi; }
void ThamGia::setNgayDK(const string& ngay) { ngayDK = ngay; }
void ThamGia::setNgayHetHan(const string& ngay) { ngayHetHan = ngay; }

string ThamGia::read() const {
    ostringstream oss;
    oss << "HoiVien: " << maHV
        << " | GoiTap: " << maGoi
        << " | Ngay DK: " << ngayDK
        << " | Het han: " << ngayHetHan;
    return oss.str();
}

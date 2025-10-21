#include "ChiTietHD.h"
#include <iomanip>

ChiTietHD::ChiTietHD() {
    
}

ChiTietHD::ChiTietHD(const string& maSP, const string& tenSP,
                     int soLuong, double donGia)
    : maSP(maSP), tenSP(tenSP), soLuong(soLuong), donGia(donGia) {}

const string& ChiTietHD::getMaSP() const { return maSP; }
const string& ChiTietHD::getTenSP() const { return tenSP; }
int ChiTietHD::getSoLuong() const { return soLuong; }
double ChiTietHD::getDonGia() const { return donGia; }

double ChiTietHD::tinhTien() const {
    return soLuong * donGia;
}

// Xác định loại sản phẩm dựa vào tiền tố ID
string ChiTietHD::getLoaiSP() const {
    if (maSP.rfind("50", 0) == 0)
        return "Gói tập";
    if (maSP.rfind("60", 0) == 0)
        return "Hàng hóa";
    return "Không xác định";
}

string ChiTietHD::read() const {
    ostringstream oss;
    oss << getLoaiSP() << ","
       << maSP << "," << tenSP << ","
       << soLuong << "," << donGia << ","
       << tinhTien();
    return oss.str();
}

#include "NhanVien.h"

NhanVien::NhanVien() {

}

NhanVien::NhanVien(const string& ID, const string& hoTen, const string& sdt, const string& gioiTinh, double luong)
    : Person(ID, hoTen, sdt, gioiTinh), luong(luong) {

}


#include "NhanVienService.h"
#include "NhanVien.h"
#include "Validator.h"
#include "QuanLy.h"
#include "MyVector.h"

void NhanVienService::themNhanVien(const string& tenNV, const string& sdt,
                                   const string& gioiTinh, const string& ngaySinh, double luong) {
    string errorMsg;
    errorMsg = Validator::validateTen(tenNV);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên nhân viên: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateSDT(sdt);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi số điện thoại: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateGioiTinh(gioiTinh);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi giới tính: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateNgay(ngaySinh);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày sinh: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateSoDuong(luong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi lương: " + errorMsg);
        return;
    }
    QuanLy& ql = QuanLy::getInstance();
    if (ql.getNhanVienBySDT(sdt) != nullptr) {
        // UI::showError("Số điện thoại đã tồn tại.");
        return;
    }
    NhanVien* newNhanVien = NhanVien::create(tenNV, sdt, gioiTinh, ngaySinh, luong);
    ql.addNhanVien(newNhanVien);
    ql.setDirty(true);
    // UI::showMessage("Thêm nhân viên thành công.");
}

void NhanVienService::suaNhanVien(const string& maNV, const string& tenNV, const string& sdt,
                                  const string& gioiTinh, const string& ngaySinh, double luong) {
    QuanLy& ql = QuanLy::getInstance();
    NhanVien* nvCanSua = ql.getNhanVien(maNV);
    if (nvCanSua == nullptr) {
        // UI::showError("Mã nhân viên không tồn tại.");
        return;
    }
    string errorMsg = Validator::validateTen(tenNV);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên nhân viên: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateSDT(sdt);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi số điện thoại: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateGioiTinh(gioiTinh);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi giới tính: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateNgay(ngaySinh);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày sinh: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateSoDuong(luong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi lương: " + errorMsg);
        return;
    }

    NhanVien* nvKiemTraSDT = ql.getNhanVienBySDT(sdt);
    if (nvKiemTraSDT != nullptr && nvKiemTraSDT->getID() != maNV) {
        // UI::showError("Số điện thoại đã tồn tại.");
        return;
    }

    nvCanSua->setHoTen(tenNV);
    nvCanSua->setSDT(sdt);
    nvCanSua->setGioiTinh(gioiTinh);
    nvCanSua->setNgaySinh(ngaySinh);
    nvCanSua->setLuong(luong);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật nhân viên thành công.");
}

void NhanVienService::xoaNhanVien(const string& maNV) {
    QuanLy& ql = QuanLy::getInstance();
    NhanVien* nvCanXoa = ql.getNhanVien(maNV);
    if (nvCanXoa == nullptr) {
        // UI::showError("Mã nhân viên không tồn tại.");
        return;
    }
    if (ql.removeNhanVien(maNV)) {
        ql.setDirty(true);
        // UI::showMessage("Xóa nhân viên thành công.");
        return;
    } else {
        // UI::showError("Lỗi: Xóa nhân viên thất bại (Lỗi hệ thống).");
        return;
    }
}

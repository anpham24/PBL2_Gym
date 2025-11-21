#include "HoiVienService.h"
#include "HoiVien.h"
#include "Validator.h"
#include "QuanLy.h"
#include "MyVector.h"

void HoiVienService::themHoiVien(const string& tenHV, const string& sdt,
                                 const string& gioiTinh, const string& ngaySinh, int point) {
    string errorMsg;
    errorMsg = Validator::validateTen(tenHV);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên hội viên: " + errorMsg);
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
    errorMsg = Validator::validateSoDuong(point);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi điểm: " + errorMsg);
        return;
    }
    QuanLy& ql = QuanLy::getInstance();
    HoiVien* newHoiVien = HoiVien::create(tenHV, sdt, gioiTinh, ngaySinh, point);

    if (ql.addHoiVien(newHoiVien)) {
        ql.setDirty(true);
        // UI::showMessage("Thêm hội viên thành công.");
    } else {
        delete newHoiVien; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể thêm hội viên (Trùng ID hoặc lỗi hệ thống).");
    }
}

void HoiVienService::suaHoiVien(const string& maHV, const string& tenHV, const string& sdt,
                                    const string& gioiTinh, const string& ngaySinh, int point) {
    QuanLy& ql = QuanLy::getInstance();
    HoiVien* hvCanSua = ql.getHoiVien(maHV);
    if (hvCanSua == nullptr) {
        // UI::showError("Mã hội viên không tồn tại.");
        return;
    }
    string errorMsg = Validator::validateTen(tenHV);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên hội viên: " + errorMsg);
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
    errorMsg = Validator::validateSoDuong(point);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi điểm: " + errorMsg);
        return;
    }

    hvCanSua->setHoTen(tenHV);
    hvCanSua->setSDT(sdt);
    hvCanSua->setGioiTinh(gioiTinh);
    hvCanSua->setNgaySinh(ngaySinh);
    hvCanSua->setPoint(point);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật hội viên thành công.");
}

void HoiVienService::xoaHoiVien(const string& maHV) {
    QuanLy& ql = QuanLy::getInstance();
    HoiVien* hvCanXoa = ql.getHoiVien(maHV);
    if (hvCanXoa == nullptr) {
        // UI::showError("Mã hội viên không tồn tại.");
        return;
    }
    if (ql.removeHoiVien(maHV)) {
        ql.setDirty(true);
        // UI::showMessage("Xóa hội viên thành công.");
        return;
    } else {
        // UI::showError("Lỗi: Xóa hội viên thất bại (Lỗi hệ thống).");
        return;
    }
}
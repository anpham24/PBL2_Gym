#include "HLVService.h"
#include "HLV.h"
#include "Validator.h"
#include "QuanLy.h"
#include "MyVector.h"

void HLVService::themHLV(const string& tenHLV, const string& sdt,
                         const string& gioiTinh, const string& ngaySinh,
                         const string& chuyenMon, double luong) {
    string errorMsg;
    errorMsg = Validator::validateTen(tenHLV);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên HLV: " + errorMsg);
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
    if (chuyenMon.empty()) {
        // UI::showError("Chuyên môn không được để trống.");
        return;
    }
    errorMsg = Validator::validateSoDuong(luong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi lương: " + errorMsg);
        return;
    }
    QuanLy& ql = QuanLy::getInstance();
    HLV* newHLV = HLV::create(tenHLV, sdt, gioiTinh, ngaySinh, chuyenMon, luong);
    if (ql.addHLV(newHLV)) {
        ql.setDirty(true);
        // UI::showMessage("Thêm HLV thành công.");
    } else {
        delete newHLV; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể thêm HLV (Trùng ID hoặc lỗi hệ thống).");
    }
}

void HLVService::suaHLV(const string& maHLV, const string& tenHLV, const string& sdt,
                        const string& gioiTinh, const string& ngaySinh,
                        const string& chuyenMon, double luong) {
    QuanLy& ql = QuanLy::getInstance();
    HLV* hlvCanSua = ql.getHLV(maHLV);
    if (hlvCanSua == nullptr) {
        // UI::showError("Mã HLV không tồn tại.");
        return;
    }
    string errorMsg = Validator::validateTen(tenHLV);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên HLV: " + errorMsg);
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
    if (chuyenMon.empty()) {
        // UI::showError("Chuyên môn không được để trống.");
        return;
    }
    errorMsg = Validator::validateSoDuong(luong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi lương: " + errorMsg);
        return;
    }

    hlvCanSua->setHoTen(tenHLV);
    hlvCanSua->setSDT(sdt);
    hlvCanSua->setGioiTinh(gioiTinh);
    hlvCanSua->setNgaySinh(ngaySinh);
    hlvCanSua->setChuyenMon(chuyenMon);
    hlvCanSua->setLuong(luong);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật HLV thành công.");
}

void HLVService::xoaHLV(const string& maHLV) {
    QuanLy& ql = QuanLy::getInstance();
    HLV* hlvCanXoa = ql.getHLV(maHLV);
    if (hlvCanXoa == nullptr) {
        // UI::showError("Mã HLV không tồn tại.");
        return;
    }
    if (ql.removeHLV(maHLV)) {
        ql.setDirty(true);
        // UI::showMessage("Xóa HLV thành công.");
        return;
    } else {
        // UI::showError("Lỗi: Xóa HLV thất bại (Lỗi hệ thống).");
        return;
    }
}

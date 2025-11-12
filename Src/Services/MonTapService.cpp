#include "MonTapService.h"
#include "MonTap.h"
#include "Validator.h"
#include "QuanLy.h"
#include "MyVector.h"

void MonTapService::themMonTap(const string& tenMon) {
    if (tenMon.empty()) {
        // UI::showError("Tên môn tập không được để trống.");
        return;
    }
    
    string errorMsg = Validator::validateTen(tenMon);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên môn tập: " + errorMsg);
        return;
    }
    
    QuanLy& ql = QuanLy::getInstance();
    MonTap* newMonTap = MonTap::create(tenMon);
    ql.addMonTap(newMonTap);
    ql.setDirty(true);
    // UI::showMessage("Thêm môn tập thành công.");
}

void MonTapService::suaMonTap(const string& maMon, const string& tenMon) {
    QuanLy& ql = QuanLy::getInstance();
    MonTap* mtCanSua = ql.getMonTap(maMon);
    if (mtCanSua == nullptr) {
        // UI::showError("Mã môn tập không tồn tại.");
        return;
    }
    if (tenMon.empty()) {
        // UI::showError("Tên môn tập không được để trống.");
        return;
    }
    string errorMsg = Validator::validateTen(tenMon);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên môn tập: " + errorMsg);
        return;
    }    
    mtCanSua->setTenMon(tenMon);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật môn tập thành công.");
}

void MonTapService::xoaMonTap(const string& maMon) {
    QuanLy& ql = QuanLy::getInstance();
    MonTap* mtCanXoa = ql.getMonTap(maMon);
    if (mtCanXoa == nullptr) {
        // UI::showError("Mã môn tập không tồn tại.");
        return;
    }
    
    if (ql.removeMonTap(maMon)) {
        ql.setDirty(true);
        // UI::showMessage("Xóa môn tập thành công.");
        return;
    } else {
        // UI::showError("Lỗi: Xóa môn tập thất bại (Lỗi hệ thống).");
        return;
    }
}

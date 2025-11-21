#include "LogTapPTService.h"
#include "LogTapPT.h"
#include "Validator.h"
#include "QuanLy.h"

void LogTapPTService::themLogTapPT(const string& maHV, const string& maHLV, const string& ngayGio) {
    string errorMsg = Validator::validateNgayGio(ngayGio);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày giờ: " + errorMsg);
        return;
    }
    
    QuanLy& ql = QuanLy::getInstance();
    HoiVien* hv = ql.getHoiVien(maHV);
    if (hv == nullptr) {
        // UI::showError("Mã hội viên không tồn tại.");
        return;
    }
    
    HLV* hlv = ql.getHLV(maHLV);
    if (hlv == nullptr) {
        // UI::showError("Mã HLV không tồn tại.");
        return;
    }
    
    LogTapPT* newLog = LogTapPT::create(ngayGio, hv, hlv);
    if (ql.addLogTapPT(newLog)) {
        ql.setDirty(true);
        // UI::showMessage("Thêm log tập PT thành công.");
    } else {
        delete newLog; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể thêm log tập PT (Trùng ID hoặc lỗi hệ thống).");
    }
}

void LogTapPTService::suaLogTapPT(const string& maLog, const string& maHV, const string& maHLV,
                                  const string& ngayGio) {
    QuanLy& ql = QuanLy::getInstance();
    LogTapPT* logCanSua = ql.getLogTapPT(maLog);
    if (logCanSua == nullptr) {
        // UI::showError("Mã log không tồn tại.");
        return;
    }
    
    string errorMsg = Validator::validateNgayGio(ngayGio);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày giờ: " + errorMsg);
        return;
    }
    
    HoiVien* hv = ql.getHoiVien(maHV);
    if (hv == nullptr) {
        // UI::showError("Mã hội viên không tồn tại.");
        return;
    }
    
    HLV* hlv = ql.getHLV(maHLV);
    if (hlv == nullptr) {
        // UI::showError("Mã HLV không tồn tại.");
        return;
    }
    
    logCanSua->setHoiVien(hv);
    logCanSua->setHLV(hlv);
    logCanSua->setNgayGio(ngayGio);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật log tập PT thành công.");
}

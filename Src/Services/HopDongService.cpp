#include "HopDongService.h"
#include "HopDong.h"
#include "Validator.h"
#include "QuanLy.h"

void HopDongService::themHopDong(const string& maHV, const string& maGT, const string& maNV,
                                 const string& ngayDK, const string& ngayHetHan) {
    string errorMsg = Validator::validateNgay(ngayDK);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày đăng ký: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateNgay(ngayHetHan);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày hết hạn: " + errorMsg);
        return;
    }

    QuanLy& ql = QuanLy::getInstance();
    HoiVien* hv = ql.getHoiVien(maHV);
    if (hv == nullptr) {
        // UI::showError("Mã hội viên không tồn tại.");
        return;
    }
    GoiTap* gt = ql.getGoiTap(maGT);
    if (gt == nullptr) {
        // UI::showError("Mã gói tập không tồn tại.");
        return;
    }
    NhanVien* nv = ql.getNhanVien(maNV);
    if (nv == nullptr) {
        // UI::showError("Mã nhân viên không tồn tại.");
        return;
    }

    HopDong* newHopDong = HopDong::create(ngayDK, ngayHetHan, true, hv, gt, nv);
    if (ql.addHopDong(newHopDong)) {
        ql.setDirty(true);
        // UI::showMessage("Thêm hợp đồng thành công.");
    } else {
        delete newHopDong; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể thêm hợp đồng (Trùng ID hoặc lỗi hệ thống).");
    }
}

void HopDongService::suaHopDong(const string& maHD, const string& maHV, const string& maGT,
                                const string& maNV, const string& ngayDK, const string& ngayHetHan) {
    QuanLy& ql = QuanLy::getInstance();
    HopDong* hdCanSua = ql.getHopDong(maHD);
    if (hdCanSua == nullptr) {
        // UI::showError("Mã hợp đồng không tồn tại.");
        return;
    }
    
    string errorMsg = Validator::validateNgay(ngayDK);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày đăng ký: " + errorMsg);
        return;
    }
    errorMsg = Validator::validateNgay(ngayHetHan);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi ngày hết hạn: " + errorMsg);
        return;
    }

    HoiVien* hv = ql.getHoiVien(maHV);
    if (hv == nullptr) {
        // UI::showError("Mã hội viên không tồn tại.");
        return;
    }
    GoiTap* gt = ql.getGoiTap(maGT);
    if (gt == nullptr) {
        // UI::showError("Mã gói tập không tồn tại.");
        return;
    }
    NhanVien* nv = ql.getNhanVien(maNV);
    if (nv == nullptr) {
        // UI::showError("Mã nhân viên không tồn tại.");
        return;
    }

    hdCanSua->setHoiVien(hv);
    hdCanSua->setGoiTap(gt);
    hdCanSua->setNhanVien(nv);
    hdCanSua->setNgayDK(ngayDK);
    hdCanSua->setNgayHetHan(ngayHetHan);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật hợp đồng thành công.");
}

void HopDongService::huyHopDong(const string& maHD) {
    QuanLy& ql = QuanLy::getInstance();
    HopDong* hd = ql.getHopDong(maHD);
    if (hd == nullptr) {
        // UI::showError("Mã hợp đồng không tồn tại.");
        return;
    }
    
    if (ql.cancelHopDong(maHD)) {
        ql.setDirty(true);
        // UI::showMessage("Hủy hợp đồng thành công.");
    } else {
        // UI::showError("Lỗi: Hủy hợp đồng thất bại.");
    }
}

void HopDongService::kichHoatHopDong(const string& maHD) {
    QuanLy& ql = QuanLy::getInstance();
    HopDong* hd = ql.getHopDong(maHD);
    if (hd == nullptr) {
        // UI::showError("Mã hợp đồng không tồn tại.");
        return;
    }
    
    hd->setIsActive(true);
    ql.setDirty(true);
    // UI::showMessage("Kích hoạt hợp đồng thành công.");
}

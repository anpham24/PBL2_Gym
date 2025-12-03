#include "HangHoaService.h"
#include "HangHoa.h"
#include "Validator.h"
#include "QuanLy.h"

void HangHoaService::themHangHoa(const string& tenHH, const string& loaiHH, double gia, int soLuong) {  
    string errorMsg = Validator::validateTen(tenHH);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên hàng hóa: " + errorMsg);
        return;
    }
    
    if (loaiHH.empty()) {
        // UI::showError("Loại hàng hóa không được để trống.");
        return;
    }
    
    errorMsg = Validator::validateSoDuong(gia);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi giá: " + errorMsg);
        return;
    }
    
    errorMsg = Validator::validateSoDuong(soLuong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi số lượng: " + errorMsg);
        return;
    }
    
    QuanLy& ql = QuanLy::getInstance();
    
    HangHoa* newHangHoa = HangHoa::create(tenHH, loaiHH, gia, soLuong);
    if (ql.addHangHoa(newHangHoa)) {
        ql.setDirty(true);
        // UI::showMessage("Thêm hàng hóa thành công.");
    } else {
        delete newHangHoa; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể thêm hàng hóa (Trùng ID hoặc lỗi hệ thống).");
        return;
    }
}

void HangHoaService::suaHangHoa(const string& maHH, const string& tenHH, const string& loaiHH,
                                double gia, int soLuong) {
    QuanLy& ql = QuanLy::getInstance();
    HangHoa* hhCanSua = ql.getHangHoa(maHH);
    if (hhCanSua == nullptr) {
        // UI::showError("Mã hàng hóa không tồn tại.");
        return;
    }
    
    if (tenHH.empty()) {
        // UI::showError("Tên hàng hóa không được để trống.");
        return;
    }
    
    string errorMsg = Validator::validateTen(tenHH);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi tên hàng hóa: " + errorMsg);
        return;
    }
    
    if (loaiHH.empty()) {
        // UI::showError("Loại hàng hóa không được để trống.");
        return;
    }
    
    errorMsg = Validator::validateSoDuong(gia);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi giá: " + errorMsg);
        return;
    }
    
    errorMsg = Validator::validateSoDuong(soLuong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi số lượng: " + errorMsg);
        return;
    }
    
    hhCanSua->setTenHH(tenHH);
    hhCanSua->setLoaiHH(loaiHH);
    hhCanSua->setGia(gia);
    hhCanSua->setSoLuongCon(soLuong);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật hàng hóa thành công.");
}

void HangHoaService::xoaHangHoa(const string& maHH) {
    QuanLy& ql = QuanLy::getInstance();
    HangHoa* hhCanXoa = ql.getHangHoa(maHH);
    if (hhCanXoa == nullptr) {
        // UI::showError("Mã hàng hóa không tồn tại.");
        return;
    }
    
    if (ql.removeHangHoa(maHH)) {
        ql.setDirty(true);
        // UI::showMessage("Xóa hàng hóa thành công.");
        return;
    } else {
        // UI::showError("Lỗi: Xóa hàng hóa thất bại (Lỗi hệ thống).");
        return;
    }
}

const MyVector<HangHoa*>& HangHoaService::getAllHangHoa() {
    QuanLy& ql = QuanLy::getInstance();
    return ql.getDsHangHoa();
}
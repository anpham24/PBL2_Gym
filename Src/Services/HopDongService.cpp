#include "HopDongService.h"
#include "HopDong.h"
#include "Validator.h"
#include "QuanLy.h"
#include <iostream>

void HopDongService::themHopDong(const string& maHV, const string& maGT, const string& maNV,
                                 const string& ngayDK, const string& ngayHetHan, int soBuoiPT) {
    std::cout << "\n🔨 ===== CREATING CONTRACT =====" << std::endl;
    std::cout << "   HV: " << maHV << std::endl;
    std::cout << "   GT: " << maGT << std::endl;
    std::cout << "   NV: " << maNV << std::endl;
    std::cout << "   Start: " << ngayDK << std::endl;
    std::cout << "   End: " << ngayHetHan << std::endl;
    
    string errorMsg = Validator::validateNgay(ngayDK);
    if (!errorMsg.empty()) {
        std::cerr << "   ❌ Invalid start date: " << errorMsg << std::endl;
        return;
    }
    
    errorMsg = Validator::validateNgay(ngayHetHan);
    if (!errorMsg.empty()) {
        std::cerr << "   ❌ Invalid end date: " << errorMsg << std::endl;
        return;
    }

    QuanLy& ql = QuanLy::getInstance();
    
    HoiVien* hv = ql.getHoiVien(maHV);
    if (hv == nullptr) {
        std::cerr << "   ❌ HoiVien not found: " << maHV << std::endl;
        return;
    }
    
    GoiTap* gt = ql.getGoiTap(maGT);
    if (gt == nullptr) {
        std::cerr << "   ❌ GoiTap not found: " << maGT << std::endl;
        return;
    }
    
    // ✅ FIX: Cho phép ADMIN tạo hợp đồng
    NhanVien* nv = nullptr;
    if (maNV != "ADMIN") {
        nv = ql.getNhanVien(maNV);
        if (nv == nullptr) {
            std::cerr << "   ❌ NhanVien not found: " << maNV << std::endl;
            return;
        }
        std::cout << "   ✅ NhanVien: " << nv->getHoTen() << std::endl;
    } else {
        std::cout << "   ⚠️ ADMIN creating contract (no NhanVien link)" << std::endl;
    }

    std::cout << "   🔨 Creating HopDong object..." << std::endl;
    HopDong* newHopDong = HopDong::create(ngayDK, ngayHetHan, true, soBuoiPT, hv, gt, nv);
    
    if (newHopDong == nullptr) {
        std::cerr << "   ❌ HopDong::create() returned nullptr!" << std::endl;
        return;
    }
    
    std::cout << "   ✅ HopDong created: " << newHopDong->getID() << std::endl;
    
    if (ql.addHopDong(newHopDong)) {
        ql.setDirty(true);
        std::cout << "   ✅ HopDong added to QuanLy" << std::endl;
        std::cout << "================================\n" << std::endl;
    } else {
        std::cerr << "   ❌ Failed to add HopDong to QuanLy!" << std::endl;
        delete newHopDong;
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

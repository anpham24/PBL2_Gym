#include "HoaDonService.h"
#include "HoaDon.h"
#include "ChiTietHoaDon_HH.h"
#include "ChiTietHoaDon_GT.h"
#include "HangHoa.h"
#include "GoiTap.h"
#include "Validator.h"
#include "QuanLy.h"

HoaDon* HoaDonService::taoHoaDon(const string& maNV, const string& maHV,
    const string& ngayLap, const string& phuongThucTT) {
    string errorMsg = Validator::validateNgay(ngayLap);
    if (!errorMsg.empty()) {
    // UI::showError("Lỗi ngày lập: " + errorMsg);
    return nullptr; // <--- SỬA Ở ĐÂY
    }

    if (phuongThucTT.empty()) {
    // UI::showError("Phương thức thanh toán không được để trống.");
    return nullptr; // <--- SỬA Ở ĐÂY
    }

    QuanLy& ql = QuanLy::getInstance();
    NhanVien* nv = ql.getNhanVien(maNV);
    if (nv == nullptr) {
    // UI::showError("Mã nhân viên không tồn tại.");
    return nullptr; // <--- SỬA Ở ĐÂY
    }
    HoiVien* hv = ql.getHoiVien(maHV);
    if (hv == nullptr) {
    // UI::showError("Mã hội viên không tồn tại.");
    return nullptr; // <--- SỬA Ở ĐÂY
    }

    HoaDon* newHoaDon = HoaDon::create(ngayLap, phuongThucTT, nv, hv);
    if (ql.addHoaDon(newHoaDon)) {
        ql.setDirty(true);
        // UI::showMessage("Tạo hóa đơn thành công.");
    } else {
        delete newHoaDon; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể tạo hóa đơn (Trùng ID hoặc lỗi hệ thống).");
        return nullptr; // <--- SỬA Ở ĐÂY
    }
    return newHoaDon; // <--- THÊM DÒNG NÀY
}

void HoaDonService::themHangHoaVaoHoaDon(const string& maHD, const string& maHH, int soLuong, double donGia) {
    QuanLy& ql = QuanLy::getInstance();
    HoaDon* hd = ql.getHoaDon(maHD);
    if (hd == nullptr) {
        // UI::showError("Mã hóa đơn không tồn tại.");
        return;
    }
    
    HangHoa* hh = ql.getHangHoa(maHH);
    if (hh == nullptr) {
        // UI::showError("Mã hàng hóa không tồn tại.");
        return;
    }
    
    string errorMsg = Validator::validateSoDuong(soLuong);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi số lượng: " + errorMsg);
        return;
    }

    errorMsg = Validator::validateSoDuong(donGia);
    if (!errorMsg.empty()) {
        // UI::showError("Lỗi đơn giá: " + errorMsg);
        return;
    }
    
    if (hh->getSoLuongCon() < soLuong) {
        // UI::showError("Không đủ hàng trong kho.");
        return;
    }

    
    // Tao chi tiet hoa don hang hoa voi nullptr de tranh vong lap trong constructor
    ChiTietHoaDon_HH* chiTiet = ChiTietHoaDon_HH::create(soLuong, donGia, nullptr, nullptr);
    
    // Thiet lap lien ket hai chieu qua setters (moi setter tu dong cap nhat ca hai phia)
    chiTiet->setHoaDon(hd);  // Them chiTiet vao hd->dsChiTietHoaDon_HH
    chiTiet->setHangHoa(hh); // Them chiTiet vao hh->dsChiTietHoaDon_HH
    
    // Cap nhat so luong ton kho (tru di so luong da ban)
    hh->setSoLuongCon(hh->getSoLuongCon() - soLuong);    ql.setDirty(true);
    // UI::showMessage("Thêm hàng hóa vào hóa đơn thành công.");
}

void HoaDonService::themGoiTapVaoHoaDon(const string& maHD, const string& maGT, int soLuong, double donGia) {
    QuanLy& ql = QuanLy::getInstance();
    HoaDon* hd = ql.getHoaDon(maHD);
    if (hd == nullptr) {
        // UI::showError("Mã hóa đơn không tồn tại.");
        return;
    }
    
    GoiTap* gt = ql.getGoiTap(maGT);
    if (gt == nullptr) {
        // UI::showError("Mã gói tập không tồn tại.");
        return;
    }
    
    // Tao chi tiet hoa don goi tap voi nullptr de tranh vong lap trong constructor
    ChiTietHoaDon_GT* chiTiet = ChiTietHoaDon_GT::create(soLuong, donGia, nullptr, nullptr);
    
    // Thiet lap lien ket hai chieu qua setters (moi setter tu dong cap nhat ca hai phia)
    chiTiet->setHoaDon(hd); // Them chiTiet vao hd->dsChiTietHoaDon_GT
    chiTiet->setGoiTap(gt); // Them chiTiet vao gt->dsChiTietHoaDon_GT
    
    ql.setDirty(true);
    // UI::showMessage("Thêm gói tập vào hóa đơn thành công.");
}

void HoaDonService::xoaHangHoaKhoiHoaDon(const string& maHD, const string& maHH) {
    QuanLy& ql = QuanLy::getInstance();
    HoaDon* hd = ql.getHoaDon(maHD);
    if (hd == nullptr) {
        // UI::showError("Mã hóa đơn không tồn tại.");
        return;
    }
    
    MyVector<ChiTietHoaDon_HH*>& dsChiTiet = hd->getDsChiTietHoaDon_HH();
    for (size_t i = 0; i < dsChiTiet.size(); ++i) {
        if (dsChiTiet[i]->getHangHoa()->getID() == maHH) {
            // Hoan lai so luong da ban vao kho (rollback inventory)
            HangHoa* hh = ql.getHangHoa(maHH);
            if (hh != nullptr) {
                hh->setSoLuongCon(hh->getSoLuongCon() + dsChiTiet[i]->getSoLuong());
            }
            
            // Xoa lien ket va giai phong bo nho
            ChiTietHoaDon_HH* itemToDelete = dsChiTiet[i]; // 1. Lưu con trỏ lại
            hd->removeChiTietHoaDon_HH(itemToDelete);      // 2. Xóa khỏi danh sách
            delete itemToDelete;
            ql.setDirty(true);
            // UI::showMessage("Xóa hàng hóa khỏi hóa đơn thành công.");
            return;
        }
    }
    // UI::showError("Không tìm thấy hàng hóa trong hóa đơn.");
}

void HoaDonService::xoaGoiTapKhoiHoaDon(const string& maHD, const string& maGT) {
    QuanLy& ql = QuanLy::getInstance();
    HoaDon* hd = ql.getHoaDon(maHD);
    if (hd == nullptr) {
        // UI::showError("Mã hóa đơn không tồn tại.");
        return;
    }
    
    MyVector<ChiTietHoaDon_GT*>& dsChiTiet = hd->getDsChiTietHoaDon_GT();
    for (size_t i = 0; i < dsChiTiet.size(); ++i) {
        if (dsChiTiet[i]->getGoiTap()->getID() == maGT) {
            // Xoa lien ket hai chieu va giai phong bo nho
            ChiTietHoaDon_GT* itemToDelete = dsChiTiet[i];
            hd->removeChiTietHoaDon_GT(itemToDelete);
            delete itemToDelete;
            ql.setDirty(true);
            // UI::showMessage("Xóa gói tập khỏi hóa đơn thành công.");
            return;
        }
    }
    // UI::showError("Không tìm thấy gói tập trong hóa đơn.");
}

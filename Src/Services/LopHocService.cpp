#include "LopHocService.h"
#include "LopHoc.h"
#include "Validator.h"
#include "QuanLy.h"

void LopHocService::themLopHoc(const string &tenLop, const string &lichTap, int thoiLuong,
                               const string &maMon, const string &maHLV)
{
    string errorMsg = Validator::validateTen(tenLop);
    if (!errorMsg.empty())
    {
        // UI::showError("Lỗi tên lớp học: " + errorMsg);
        return;
    }
    if (lichTap.empty())
    {
        // UI::showError("Lịch tập không được để trống.");
        return;
    }
    errorMsg = Validator::validateSoDuong(thoiLuong);
    if (!errorMsg.empty())
    {
        // UI::showError("Lỗi thời lượng: " + errorMsg);
        return;
    }

    QuanLy &ql = QuanLy::getInstance();
    MonTap *monTap = ql.getMonTap(maMon);
    HLV *hlv = ql.getHLV(maHLV);

    LopHoc *newLopHoc = LopHoc::create(tenLop, lichTap, thoiLuong, monTap, hlv);
    if (ql.addLopHoc(newLopHoc))
    {
        ql.setDirty(true);
        // UI::showMessage("Thêm lớp học thành công.");
    }
    else
    {
        delete newLopHoc; // Xóa nếu thêm thất bại để tránh leak
        // UI::showError("Lỗi: Không thể thêm lớp học (Trùng ID hoặc lỗi hệ thống).");
    }
}

void LopHocService::suaLopHoc(const string &maLop, const string &tenLop, const string &lichTap,
                              int thoiLuong, const string &maMon, const string &maHLV)
{
    QuanLy &ql = QuanLy::getInstance();
    LopHoc *lopCanSua = ql.getLopHoc(maLop);
    if (lopCanSua == nullptr)
    {
        // UI::showError("Mã lớp học không tồn tại.");
        return;
    }
    string errorMsg = Validator::validateTen(tenLop);
    if (!errorMsg.empty())
    {
        // UI::showError("Lỗi tên lớp học: " + errorMsg);
        return;
    }
    if (lichTap.empty())
    {
        // UI::showError("Lịch tập không được để trống.");
        return;
    }
    errorMsg = Validator::validateSoDuong(thoiLuong);
    if (!errorMsg.empty())
    {
        // UI::showError("Lỗi thời lượng: " + errorMsg);
        return;
    }

    MonTap *monTap = ql.getMonTap(maMon);
    HLV *hlv = ql.getHLV(maHLV);

    lopCanSua->setTenLop(tenLop);
    lopCanSua->setLichTap(lichTap);
    lopCanSua->setThoiLuong(thoiLuong);
    lopCanSua->setMonTap(monTap);
    lopCanSua->setHLV(hlv);
    ql.setDirty(true);
    // UI::showMessage("Cập nhật lớp học thành công.");
}

void LopHocService::xoaLopHoc(const string &maLop)
{
    QuanLy &ql = QuanLy::getInstance();
    LopHoc *lopCanXoa = ql.getLopHoc(maLop);
    if (lopCanXoa == nullptr)
    {
        // UI::showError("Mã lớp học không tồn tại.");
        return;
    }

    if (ql.removeLopHoc(maLop))
    {
        ql.setDirty(true);
        // UI::showMessage("Xóa lớp học thành công.");
        return;
    }
    else
    {
        // UI::showError("Lỗi: Xóa lớp học thất bại (Lỗi hệ thống).");
        return;
    }
}
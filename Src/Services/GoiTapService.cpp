#include "GoiTapService.h"
#include "GoiTap.h"
#include "MonTap.h"
#include "QuanLy.h"
#include "Validator.h"

void GoiTapService::themGoiTap(const string& tenGoi, int thoiGian, int soBuoiPT, double gia) {
    string errorMsg = Validator::validateTen(tenGoi);
    if (!errorMsg.empty()) {
        // UI::showError(errorMsg)
        return;
    }
    errorMsg = Validator::validateSoDuong(thoiGian);
    if (!errorMsg.empty()) {
        //
        return;
    }
    errorMsg = Validator::validateSoDuong(soBuoiPT);
    if (!errorMsg.empty()) {
        //
        return;
    }
    errorMsg = Validator::validateSoDuong(gia);
    if (!errorMsg.empty()) {
        //
        return;
    }

    GoiTap* newGoiTap = GoiTap::create(tenGoi, thoiGian, soBuoiPT, gia);
    QuanLy& ql = QuanLy::getInstance();
    ql.addGoiTap(newGoiTap);
    ql.setDirty(true);
    // UI::Thêm thành công
}

void GoiTapService::themMonTapVaoGoiTap(const string& maGoi, const string& maMon) {
    QuanLy& ql = QuanLy::getInstance();
    GoiTap* goiTap = ql.getGoiTap(maGoi);
    if (goiTap == nullptr) {
        // Lỗi không tìm thấy gói tập
        return;
    }
    MonTap* monTap = ql.getMonTap(maMon);
    if (monTap == nullptr) {
        // Lỗi không tìm thấy môn tập
        return;
    }
    // Kiểm tra xem môn tập đã có trong gói tập chưa
    const MyVector<MonTap*>& dsMonTap = goiTap->getDsMonTap();
    for (size_t i = 0; i < dsMonTap.size(); ++i) {
        if (dsMonTap[i]->getID() == maMon) {
            // Môn tập đã có trong gói tập
            return;
        }
    }
    goiTap->addMonTap(monTap);
    ql.setDirty(true);
    // UI::Thêm môn tập vào gói tập thành công
}

void GoiTapService::suaGoiTap(const string& maGoi, const string& tenGoi, int thoiGian, int soBuoiPT, double gia) {
    QuanLy& ql = QuanLy::getInstance();
    GoiTap* gtCanSua = ql.getGoiTap(maGoi);
    if (gtCanSua == nullptr) {
        // Lỗi không tìm thấy gói tập
        return;
    }
    string errorMsg = Validator::validateTen(tenGoi);
    if (!errorMsg.empty()) {
        // UI::showError(errorMsg)
        return;
    }
    errorMsg = Validator::validateSoDuong(thoiGian);
    if (!errorMsg.empty()) {
        //
        return;
    }
    errorMsg = Validator::validateSoDuong(soBuoiPT);
    if (!errorMsg.empty()) {
        //
        return;
    }
    errorMsg = Validator::validateSoDuong(gia);
    if (!errorMsg.empty()) {
        //
        return;
    }
    gtCanSua->setTenGoi(tenGoi);
    gtCanSua->setThoiGian(thoiGian);
    gtCanSua->setSoBuoiPT(soBuoiPT);
    gtCanSua->setGia(gia);
    ql.setDirty(true);
    // UI::Sửa thành công
}

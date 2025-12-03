#include "BaoCaoService.h"
#include "QuanLy.h"
#include "HoaDon.h"
#include "NhanVien.h"
#include "ChiTietHoaDon_GT.h"
#include "ChiTietHoaDon_HH.h"
#include "GoiTap.h"
#include "HangHoa.h"
#include "DateTimeUils.h"
#include "Validator.h"
#include <iostream>

ReportResult BaoCaoService::getReport(const std::string &fromDate, const std::string &toDate, const std::string &staffID)
{
    ReportResult result;

    // 1. Validate ngày
    if (Validator::validateNgay(fromDate) != "" || Validator::validateNgay(toDate) != "")
    {
        result.errorMsg = "Dinh dang ngay khong hop le (DD/MM/YYYY)";
        return result;
    }

    // 2. Lấy dữ liệu từ Quản Lý
    MyVector<HoaDon *> allHoaDon = QuanLy::getInstance().getDsHoaDon().getAllValues();

    // 3. Duyệt và tính toán
    for (size_t i = 0; i < allHoaDon.size(); ++i)
    {
        HoaDon *hd = allHoaDon[i];

        // Chỉ tính hóa đơn đã thanh toán
        if (hd == nullptr || !hd->getDaThanhToan())
            continue;

        // Lọc theo Nhân Viên
        if (!staffID.empty())
        {
            if (hd->getNhanVien() == nullptr || hd->getNhanVien()->getID() != staffID)
            {
                continue;
            }
        }

        // Lọc theo Ngày
        std::string ngayLap = hd->getNgayLap();
        if (!DateTimeUtils::isBetween(ngayLap, fromDate, toDate))
        {
            continue;
        }

        // --- TÍNH TOÁN CHÍNH ---
        double hdTotal = hd->getFinalTotal();
        result.totalRevenue += hdTotal;

        // Biểu đồ tháng
        try
        {
            int month = std::stoi(ngayLap.substr(3, 2));
            if (month >= 1 && month <= 12)
            {
                result.monthlyRevenue[month - 1] += hdTotal;
            }
        }
        catch (...)
        {
        }

        // Doanh thu Nhân Viên (Tổng hợp)
        if (hd->getNhanVien() != nullptr)
        {
            std::string nvName = hd->getNhanVien()->getHoTen();
            bool found = false;
            for (size_t j = 0; j < result.topNhanVien.size(); ++j)
            {
                if (result.topNhanVien[j].name == nvName)
                {
                    result.topNhanVien[j].revenue += hdTotal;
                    found = true;
                    break;
                }
            }
            if (!found)
                result.topNhanVien.push_back(RevenueItem(nvName, hdTotal));
        }

        // Doanh thu Gói Tập
        const auto &dsGT = hd->getDsChiTietHoaDon_GT();
        for (size_t j = 0; j < dsGT.size(); ++j)
        {
            auto *ct = dsGT[j];
            double ctTotal = ct->tinhTien();
            result.revenueGoiTap += ctTotal;

            std::string name = ct->getGoiTap()->getTenGoi();
            bool found = false;
            for (size_t k = 0; k < result.topGoiTap.size(); ++k)
            {
                if (result.topGoiTap[k].name == name)
                {
                    result.topGoiTap[k].revenue += ctTotal;
                    found = true;
                    break;
                }
            }
            if (!found)
                result.topGoiTap.push_back(RevenueItem(name, ctTotal));
        }

        // Doanh thu Hàng Hóa
        const auto &dsHH = hd->getDsChiTietHoaDon_HH();
        for (size_t j = 0; j < dsHH.size(); ++j)
        {
            auto *ct = dsHH[j];
            double ctTotal = ct->tinhTien();
            result.revenueHangHoa += ctTotal;

            std::string name = ct->getHangHoa()->getTenHH();
            bool found = false;
            for (size_t k = 0; k < result.topHangHoa.size(); ++k)
            {
                if (result.topHangHoa[k].name == name)
                {
                    result.topHangHoa[k].revenue += ctTotal;
                    found = true;
                    break;
                }
            }
            if (!found)
                result.topHangHoa.push_back(RevenueItem(name, ctTotal));
        }
    }

    // 4. Sắp xếp Top
    sortRevenueDesc(result.topNhanVien);
    sortRevenueDesc(result.topGoiTap);
    sortRevenueDesc(result.topHangHoa);

    return result;
}

void BaoCaoService::sortRevenueDesc(MyVector<RevenueItem> &items)
{
    // Insertion sort đơn giản cho MyVector
    for (size_t i = 1; i < items.size(); ++i)
    {
        RevenueItem key = items[i];
        int j = (int)i - 1;
        while (j >= 0 && items[j].revenue < key.revenue)
        {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = key;
    }
}
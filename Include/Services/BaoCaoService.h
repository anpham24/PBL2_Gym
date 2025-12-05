#pragma once
#include <string>
#include <vector>
#include "MyVector.h"

// Struct lưu trữ thông tin doanh thu của 1 đối tượng (Tên + Tiền)
struct RevenueItem
{
    std::string name;
    double revenue;
    RevenueItem(std::string n = "", double r = 0.0) : name(n), revenue(r) {}
};

// Struct chứa TOÀN BỘ kết quả báo cáo trả về cho UI
struct ReportResult
{
    double totalRevenue = 0;
    double revenueGoiTap = 0;
    double revenueHangHoa = 0;

    // Dữ liệu biểu đồ (12 tháng)
    std::vector<double> monthlyRevenue;

    // Các danh sách Top (đã sắp xếp)
    MyVector<RevenueItem> topGoiTap;
    MyVector<RevenueItem> topHangHoa;
    MyVector<RevenueItem> topNhanVien;

    std::string errorMsg; // Lưu thông báo lỗi nếu có (VD: Sai định dạng ngày)

    ReportResult()
    {
        monthlyRevenue.resize(12, 0.0);
    }
};

class BaoCaoService
{
public:
    static ReportResult getReport(const std::string &fromDate, const std::string &toDate, const std::string &staffID);

private:
    // Helper để sắp xếp giảm dần
    static void sortRevenueDesc(MyVector<RevenueItem> &items);
};
#pragma once
#include <string>
#include <sstream>
using namespace std;

class ThamGia {
private:
    string maHV;        // Mã hội viên
    string maGoi;       // Mã gói tập
    string ngayDK;      // Ngày đăng ký
    string ngayHetHan;  // Ngày hết hạn

public:
    ThamGia();
    ThamGia(const string& maHV, const string& maGoi,
            const string& ngayDK, const string& ngayHetHan);

    string getMaHV() const;
    string getMaGoi() const;
    string getNgayDK() const;
    string getNgayHetHan() const;

    void setMaHV(const string& hv);
    void setMaGoi(const string& goi);
    void setNgayDK(const string& ngay);
    void setNgayHetHan(const string& ngay);

    string read() const; 
};

#pragma once
#include <string>
using namespace std;

class ThamGia {
private:
    string maHV;        // Mã hội viên
    string maGoi;       // Mã gói tập
    string ngayDK;      // Ngày đăng ký
    string ngayHetHan;  // Ngày hết hạn

public:
    ThamGia();
    ThamGia(const string&, const string&, const string&, const string&);
    ThamGia(const ThamGia& other);
    ~ThamGia();

    const string& getMaHV() const;
    const string& getMaGoi() const;
    const string& getNgayDK() const;
    const string& getNgayHetHan() const;

    void setMaHV(const string& hv);
    void setMaGoi(const string& goi);
    void setNgayDK(const string& ngay);
    void setNgayHetHan(const string& ngay);

    ThamGia create(const string&, const string&, const string&, const string&);
    string read() const; 
    void update(const string&, const string&, const string&, const string&);
};

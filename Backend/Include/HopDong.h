#pragma once
#include <string>
using namespace std;

class HoiVien;
class GoiTap;
class NhanVien;

class HopDong {
private:
    string id;
    HoiVien* hv;   // Hội viên ký hợp đồng
    GoiTap* gt;   // Gói tập được đăng ký
    NhanVien* nv; // Nhân viên xử lý hợp đồng
    string ngayDK;      // Ngày đăng ký
    string ngayHetHan;  // Ngày hết hạn

public:
    HopDong();
    HopDong(HoiVien*, GoiTap*, NhanVien*, const string&, const string&);
    HopDong(const HopDong&);
    ~HopDong();

    const string& getNgayDK() const;
    const string& getNgayHetHan() const;
    const HoiVien& getHoiVien() const;
    const GoiTap& getGoiTap() const;
    const NhanVien& getNhanVien() const;

    void setNgayDK(const string&);
    void setNgayHetHan(const string&);
    void setHoiVien(HoiVien*);
    void setGoiTap(GoiTap*);
    void setNhanVien(NhanVien*);

    static HopDong* create(HoiVien*, GoiTap*, NhanVien*, const string&, const string&);
    string read() const;
};

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
    string ngayDK;      // Ngày đăng ký (format: "DD/MM/YYYY")
    string ngayHetHan;  // Ngày hết hạn (format: "DD/MM/YYYY")
    bool isActive;      // Trạng thái hợp đồng

public:
    HopDong();
    HopDong(HoiVien*, GoiTap*, NhanVien*, const string&, const string&, bool = true);
    HopDong(const HopDong&);
    ~HopDong();

    const string& getID() const;
    const string& getNgayDK() const;
    const string& getNgayHetHan() const;
    const HoiVien& getHoiVien() const;
    const GoiTap& getGoiTap() const;
    const NhanVien& getNhanVien() const;
    bool getIsActive();

    void setNgayDK(const string&);
    void setNgayHetHan(const string&);
    void setHoiVien(HoiVien*);
    void setGoiTap(GoiTap*);
    void setNhanVien(NhanVien*);
    void setIsActive(bool);

    static HopDong* create(HoiVien*, GoiTap*, NhanVien*, const string&, const string&, bool = true);
    string read();
};

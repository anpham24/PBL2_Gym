#pragma once
#include <string>
using namespace std;

class HoiVien;
class GoiTap;
class NhanVien;

class HopDong {
private:
    string id;
    string ngayDK;
    string ngayHetHan;
    bool isActive;
    int soBuoiPTConLai; // ✅ THÊM: Số buổi PT riêng của gói này
    HoiVien* hv;
    GoiTap* gt;
    NhanVien* nv;

public:
    HopDong();
    HopDong(const string&, const string&, const string&, bool = true, int = 0, 
            HoiVien* = nullptr, GoiTap* = nullptr, NhanVien* = nullptr);
    HopDong(const HopDong&);
    ~HopDong();

    const string& getID() const;
    const string& getNgayDK() const;
    const string& getNgayHetHan() const;
    const HoiVien& getHoiVien() const;
    const GoiTap& getGoiTap() const;
    const NhanVien& getNhanVien() const;
    bool getIsActive();
    
    // ✅ THÊM getter/setter
    int getSoBuoiPTConLai() const;
    void setSoBuoiPTConLai(int);
    void truBuoiPT(int soLuong = 1); // ✅ Trừ khi dùng PT

    void setNgayDK(const string&);
    void setNgayHetHan(const string&);
    void setHoiVien(HoiVien*);
    void setGoiTap(GoiTap*);
    void setNhanVien(NhanVien*);
    void setIsActive(bool);

    static HopDong* create(const string&, const string&, const string&, bool = true, int = 0,
                          HoiVien* = nullptr, GoiTap* = nullptr, NhanVien* = nullptr);
    static HopDong* create(const string&, const string&, bool = true, int = 0,
                          HoiVien* = nullptr, GoiTap* = nullptr, NhanVien* = nullptr);
    string read();
};
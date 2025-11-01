#pragma once
#include "Person.h"

class HopDong;
class HoaDon;

class NhanVien : public Person {
private:
    double luong;
    MyVector<HopDong*> dsHopDong; // Danh sách hợp đồng do nhân viên này xử lý
    MyVector<HoaDon*> dsHoaDon;     // Danh sách hóa đơn do nhân viên này lập
public:
    NhanVien();
    NhanVien(const string&, const string&, const string&, int, double = 0.0);
    NhanVien(const NhanVien& other);
    ~NhanVien();

    double getLuong() const;
    void setLuong(double);

    void addHopDong(HopDong*);
    void removeHopDong(HopDong*);
    const MyVector<HopDong*>& getDsHopDong() const;
    MyVector<HopDong*>& getDsHopDong();

    void addHoaDon(HoaDon*);
    void removeHoaDon(HoaDon*);
    const MyVector<HoaDon*>& getDsHoaDon() const;
    MyVector<HoaDon*>& getDsHoaDon();

    static NhanVien* create(const string&, const string&, const string&, int, double);
    string read() const override;
};
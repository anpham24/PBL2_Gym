#pragma once
#include "Person.h"
#include "MyVector.h"
#include <string>

class HopDong;
class HoaDon;

class NhanVien : public Person {
private:
    double luong;
    MyVector<HopDong*> dsHopDong; // Danh sách hợp đồng do nhân viên này xử lý
    MyVector<HoaDon*> dsHoaDon;     // Danh sách hóa đơn do nhân viên này lập
    bool isActive;
public:
    NhanVien();
    NhanVien(const string&, const string&, const string&, const string&, int, double = 0.0, bool = true);
    NhanVien(const NhanVien& other);
    ~NhanVien();

    double getLuong() const;
    bool getIsActive() const;
    void setLuong(double);
    void setIsActive(bool);

    void addHopDong(HopDong*);
    void removeHopDong(HopDong*);
    const MyVector<HopDong*>& getDsHopDong() const;
    MyVector<HopDong*>& getDsHopDong();

    void addHoaDon(HoaDon*);
    void removeHoaDon(HoaDon*);
    const MyVector<HoaDon*>& getDsHoaDon() const;
    MyVector<HoaDon*>& getDsHoaDon();

    static NhanVien* create(const string&, const string&, const string&, const string&, int, double = 0.0, bool = true);
    static NhanVien* create(const string&, const string&, const string&, int, double = 0.0, bool = true);
    string read() const override;
};
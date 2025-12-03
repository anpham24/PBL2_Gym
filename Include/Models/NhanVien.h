#pragma once
#include <string>

#include "Person.h"
#include "MyVector.h"

class HopDong;
class HoaDon;

class NhanVien : public Person {
private:
    double luong;
    bool isActive;
    MyVector<HopDong*> dsHopDong; // Danh sách hợp đồng do nhân viên này xử lý
    MyVector<HoaDon*> dsHoaDon;     // Danh sách hóa đơn do nhân viên này lập
public:
    NhanVien();
    NhanVien(const string&, const string&, const string&, const string&, const string&, double = 0.0, bool = true);
    NhanVien(const NhanVien& other);
    ~NhanVien();

    // Getters
    double getLuong() const;
    bool getIsActive() const;

    // Setters
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

    static NhanVien* create(const string&, const string&, const string&, const string&, const string&, double = 0.0, bool = true);
    static NhanVien* create(const string&, const string&, const string&, const string&, double = 0.0, bool = true);
    string read() const override;
};
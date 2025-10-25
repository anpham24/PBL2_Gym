#pragma once
#include "Person.h"

class NhanVien : public Person {
private:
    double luong;
public:
    NhanVien();
    NhanVien(const string&, const string&, const string&, double = 0.0);
    NhanVien(const NhanVien& other);
    ~NhanVien();

    double getLuong() const;
    void setLuong(double);

    static NhanVien create(const string&, const string&, const string&, double);
    string read() const override;
    void update(const string&, const string&, const string&, double);
};
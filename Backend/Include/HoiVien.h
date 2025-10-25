#pragma once
#include "Person.h"

class HoiVien : public Person {
private:
    int point;
    string maHLV;
public:
    HoiVien();
    HoiVien(const string&, const string&, const string&, int, int = 0, const string& = "");
    HoiVien(const HoiVien& other);
    ~HoiVien();

    const string& getMaHLV() const;
    double getPoint() const;
    void setPoint(int);
    void setMaHLV(const string&);

    static HoiVien create(const string&, const string&, const string&, int, int = 0, const string& = "");
    string read() const override;
    void update(const string&, const string&, const string&, int, int, const string& = "");
};
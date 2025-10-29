#pragma once
#include "Person.h"
#include "MyVector.h"

class HLV; // Forward declaration
class HoaDon; // Forward declaration
class HopDong; // Forward declaration

class HoiVien : public Person {
private:
    int point;
    HLV* hlv;
    MyVector<HoaDon*> dsHoaDon;
    MyVector<HopDong*> dsHopDong;
public:
    HoiVien();
    HoiVien(const string&, const string&, const string&, int, int = 0, HLV* = nullptr);
    HoiVien(const HoiVien& other);
    ~HoiVien();

    const HLV* getHLV() const;
    double getPoint() const;
    void setPoint(int);
    void setHLV(const HLV*);
    void removeHLV();

    void addHoaDon(HoaDon*);
    void removeHoaDon(HoaDon*);
    const MyVector<HoaDon*>& getDsHoaDon() const;

    void addHopDong(HopDong*);
    void removeHopDong(HopDong*);
    const MyVector<HopDong*>& getDsHopDong() const;

    static HoiVien* create(const string&, const string&, const string&, int, int, HLV*);
    string read() const override;
};
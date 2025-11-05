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
    bool isActive;
public:
    HoiVien();
    HoiVien(const string&, const string&, const string&, int, int = 0, HLV* = nullptr, bool = true);
    HoiVien(const HoiVien& other);
    ~HoiVien();

    const HLV* getHLV() const;
    double getPoint() const;
    void setPoint(int);
    void setHLV(HLV*);
    bool getIsActive() const;
    void setIsActive(bool);

    void addHoaDon(HoaDon*);
    void removeHoaDon(HoaDon*);
    const MyVector<HoaDon*>& getDsHoaDon() const;
    MyVector<HoaDon*>& getDsHoaDon();

    void addHopDong(HopDong*);
    void removeHopDong(HopDong*);
    const MyVector<HopDong*>& getDsHopDong() const;
    MyVector<HopDong*>& getDsHopDong();

    static HoiVien* create(const string&, const string&, const string&, int, int, HLV*, bool = true);
    string read() const override;
};
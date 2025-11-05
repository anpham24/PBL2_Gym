#pragma once
#include "MyVector.h"
#include <string>
using namespace std;

class ChiTietHoaDon_HH;

class HangHoa {
private:
    string id;
    string tenHH;
    double gia;
    int soLuongCon;
    MyVector<ChiTietHoaDon_HH*> dsChiTietHoaDon_HH;
    bool isActive;
public:
    HangHoa();
    HangHoa(const string&, double, int, bool = true);
    HangHoa(const HangHoa& other);
    ~HangHoa();

    const string& getID() const;
    const string& getTenHH() const;
    double getGia() const;
    int getSoLuongCon() const;
    bool getIsActive() const;

    void setTenHH(const string&);
    void setGia(double);
    void setSoLuongCon(int);
    void setIsActive(bool);

    void addChiTietHoaDon_HH(ChiTietHoaDon_HH*);
    void removeChiTietHoaDon_HH(ChiTietHoaDon_HH*);
    const MyVector<ChiTietHoaDon_HH*>& getDsChiTietHoaDon_HH() const;
    MyVector<ChiTietHoaDon_HH*>& getDsChiTietHoaDon_HH();

    static HangHoa* create(const string&, double, int, bool = true);
    string read() const;
};
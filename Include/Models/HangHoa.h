#pragma once
#include <string>

#include "MyVector.h"

using namespace std;

class ChiTietHoaDon_HH;

class HangHoa {
private:
    string id;
    string tenHH;
    string loaiHH; // e.g., "Thuc Pham", "Thiet Bi", etc.
    double gia;
    int soLuongCon;
    bool isActive;
    MyVector<ChiTietHoaDon_HH*> dsChiTietHoaDon_HH;
public:
    HangHoa();
    HangHoa(const string&, const string&, const string&, double, int, bool = true);
    HangHoa(const HangHoa& other);
    ~HangHoa();

    const string& getID() const;
    const string& getTenHH() const;
    const string& getLoaiHH() const;
    double getGia() const;
    int getSoLuongCon() const;
    bool getIsActive() const;

    void setTenHH(const string&);
    void setLoaiHH(const string&);
    void setGia(double);
    void setSoLuongCon(int);
    void setIsActive(bool);

    void addChiTietHoaDon_HH(ChiTietHoaDon_HH*);
    void removeChiTietHoaDon_HH(ChiTietHoaDon_HH*);
    const MyVector<ChiTietHoaDon_HH*>& getDsChiTietHoaDon_HH() const;
    MyVector<ChiTietHoaDon_HH*>& getDsChiTietHoaDon_HH();

    static HangHoa* create(const string&, const string&, const string&, double, int, bool = true);
    static HangHoa* create(const string&, const string&, double, int, bool = true);
    string read() const;
};
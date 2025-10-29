#pragma once
#include "MyVector.h"
#include <string>
using namespace std;

class NhanVien;
class HoiVien;
class ChiTietHoaDon_HH;
class ChiTietHoaDon_GT;

class HoaDon {
private:
    string id;
    NhanVien* nhanVien;
    HoiVien* hoiVien;
    string ngayLap;
    string phuongThucTT;
    MyVector<ChiTietHoaDon_HH*> itemsHH;
    MyVector<ChiTietHoaDon_GT*> itemsGT;
public:
    HoaDon();
    HoaDon(NhanVien*, HoiVien*, const string&, const string&);
    HoaDon(const HoaDon& other);
    ~HoaDon();

    const string& getID() const;
    const NhanVien* getNhanVien() const;
    const HoiVien* getHoiVien() const;
    const string& getNgayLap() const;
    const string& getPhuongThuc() const;
    const MyVector<ChiTietHoaDon_HH*>& getItemsHH() const;
    const MyVector<ChiTietHoaDon_GT*>& getItemsGT() const;

    void setNhanVien(NhanVien*);
    void setHoiVien(HoiVien*);
    void setNgayLap(const string&);
    void setPhuongThucTT(const string&);

    void addItemHH(ChiTietHoaDon_HH*);
    void removeItemHH(ChiTietHoaDon_HH*);

    void addItemGT(ChiTietHoaDon_GT*);
    void removeItemGT(ChiTietHoaDon_GT*);

    double getTotal() const;
    size_t itemCount() const;

    static HoaDon* create(NhanVien*, HoiVien*, const string&, const string&);
    string read() const;
};
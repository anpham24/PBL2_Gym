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
    MyVector<ChiTietHoaDon_HH*> dsChiTietHoaDon_HH;
    MyVector<ChiTietHoaDon_GT*> dsChiTietHoaDon_GT;
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
    const MyVector<ChiTietHoaDon_HH*>& getDsChiTietHoaDon_HH() const;
    MyVector<ChiTietHoaDon_HH*>& getDsChiTietHoaDon_HH();
    const MyVector<ChiTietHoaDon_GT*>& getDsChiTietHoaDon_GT() const;
    MyVector<ChiTietHoaDon_GT*>& getDsChiTietHoaDon_GT();

    void setNhanVien(NhanVien*);
    void setHoiVien(HoiVien*);
    void setNgayLap(const string&);
    void setPhuongThucTT(const string&);

    void addHangHoa(ChiTietHoaDon_HH*);
    void removeHangHoa(ChiTietHoaDon_HH*);

    void addGoiTap(ChiTietHoaDon_GT*);
    void removeGoiTap(ChiTietHoaDon_GT*);

    double getTotal() const;
    size_t itemCount() const;

    static HoaDon* create(NhanVien*, HoiVien*, const string&, const string&);
    string read() const;
};
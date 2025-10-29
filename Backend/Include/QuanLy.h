#pragma once
#include "MyVector.h"
#include "MyHashTable.h"
#include <string>
using namespace std;

class HoiVien;
class HLV;
class NhanVien;
class GoiTap;
class LopHoc;
class MonTap;
class HopDong;
class HangHoa;
class HoaDon;

class QuanLy {
private:
    MyHashTable<HoiVien*> dsHoiVien;
    MyVector<HLV*> dsHLV;
    MyVector<NhanVien*> dsNhanVien;
    MyVector<GoiTap*> dsGoiTap;
    MyVector<LopHoc*> dsLopHoc;
    MyVector<MonTap*> dsMonTap;
    MyVector<HopDong*> dsHopDong;
    MyVector<HangHoa*> dsHangHoa;
    MyVector<HoaDon*> dsHoaDon;

    QuanLy() {}
    QuanLy(const QuanLy&) = delete;
    QuanLy& operator=(const QuanLy&) = delete;

public:
    static QuanLy& getInstance() {
        static QuanLy instance;
        return instance;
    }

    ~QuanLy();

    // Ham quan ly HoiVien
    bool themHoiVien(HoiVien*);
    bool xoaHoiVien(const string&);
    HoiVien* timHoiVien(const string&) const;

    // Ham quan ly HLV
    bool themHLV(HLV*);
    bool xoaHLV(const string&);
    HLV* timHLV(const string&) const;

    // Ham quan ly NhanVien
    bool themNhanVien(NhanVien*);
    bool xoaNhanVien(const string&);
    NhanVien* timNhanVien(const string&) const;

    // Ham quan ly GoiTap
    bool themGoiTap(GoiTap*);
    bool xoaGoiTap(const string&);
    GoiTap* timGoiTap(const string&) const;
    
    // Ham quan ly LopHoc
    bool themLopHoc(LopHoc*);
    bool xoaLopHoc(const string&);
    LopHoc* timLopHoc(const string&) const;

    // Ham quan ly MonTap
    bool themMonTap(MonTap*);
    bool xoaMonTap(const string&);
    MonTap* timMonTap(const string&) const;

    // Ham quan ly HopDong
    bool themHopDong(HopDong*);
    bool xoaHopDong(const string&);
    HopDong* timHopDong(const string&) const;

    // Ham quan ly HangHoa
    bool themHangHoa(HangHoa*);
    bool xoaHangHoa(const string&);
    HangHoa* timHangHoa(const string&) const;

    // Ham quan ly HoaDon
    bool themHoaDon(HoaDon*);
    bool xoaHoaDon(const string&);
    HoaDon* timHoaDon(const string&) const;

    void saveAllData(const string& filePath) const;
    void loadAllData(const string& filePath);

    string getDisplayList(const string& entityName) const;
};

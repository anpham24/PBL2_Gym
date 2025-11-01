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

    // Getter cho Hội Viên (MyHashTable)
    const MyHashTable<HoiVien*>& getDsHoiVien() const;
    
    // Getter cho HLV (MyVector)
    const MyVector<HLV*>& getDsHLV() const;

    // Getter cho Nhân Viên (MyVector)
    const MyVector<NhanVien*>& getDsNhanVien() const;

    // Getter cho Gói Tập (MyVector)
    const MyVector<GoiTap*>& getDsGoiTap() const;

    // Getter cho Hàng Hóa (MyVector)
    const MyVector<HangHoa*>& getDsHangHoa() const;

    // Getter cho Hóa Đơn (MyVector)
    const MyVector<HoaDon*>& getDsHoaDon() const;

    // Ham quan ly HoiVien
    bool addHoiVien(HoiVien*);
    bool removeHoiVien(const string&);
    const HoiVien* getHoiVien(const string&) const;

    // Ham quan ly HLV
    bool addHLV(HLV*);
    bool xoaHLV(const string&);
    HLV* timHLV(const string&) const;

    // Ham quan ly NhanVien
    bool addNhanVien(NhanVien*);
    bool xoaNhanVien(const string&);
    NhanVien* timNhanVien(const string&) const;

    // Ham quan ly GoiTap
    bool addGoiTap(GoiTap*);
    bool xoaGoiTap(const string&);
    GoiTap* timGoiTap(const string&) const;
    
    // Ham quan ly LopHoc
    bool addLopHoc(LopHoc*);
    bool removeLopHoc(const string&);
    LopHoc* getLopHoc(const string&) const;

    // Ham quan ly MonTap
    bool addMonTap(MonTap*);
    bool removeMonTap(const string&);
    MonTap* getMonTap(const string&) const;

    // Ham quan ly HopDong
    bool addHopDong(HopDong*);
    bool removeHopDong(const string&);
    HopDong* getHopDong(const string&) const;

    // Ham quan ly HangHoa
    bool addHangHoa(HangHoa*);
    bool removeHangHoa(const string&);
    HangHoa* getHangHoa(const string&) const;

    // Ham quan ly HoaDon
    bool addHoaDon(HoaDon*);
    bool removeHoaDon(const string&);
    HoaDon* getHoaDon(const string&) const;

    void saveAllData(const string& filePath) const;
    void loadAllData(const string& filePath);

    string getDisplayList(const string& entityName) const;
};

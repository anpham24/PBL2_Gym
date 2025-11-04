#pragma once
#include "../../Utils/Include/MyVector.h"
#include "../../Utils/Include/MyHashTable.h"
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
    static QuanLy* instance;
    MyHashTable<HoiVien*> dsHoiVien;
    MyVector<HLV*> dsHLV;
    MyVector<NhanVien*> dsNhanVien;
    MyVector<GoiTap*> dsGoiTap;
    MyVector<LopHoc*> dsLopHoc;
    MyVector<MonTap*> dsMonTap;
    MyHashTable<HopDong*> dsHopDong;
    MyVector<HangHoa*> dsHangHoa;
    MyHashTable<HoaDon*> dsHoaDon;

    bool isDirty = false;

    QuanLy() : dsHoiVien(7013), dsHoaDon(40009), dsHopDong(40009) {}
    QuanLy(const QuanLy&) = delete;
    QuanLy& operator=(const QuanLy&) = delete;

public:
    static QuanLy& getInstance() {
        static QuanLy instance;
        return instance;
    }

    ~QuanLy();

    void saveAllData(const string& filePath) const;
    void loadAllData(const string& filePath);

    void setDirty(bool status = true);
    bool getIsDirty() const;    

    // Ham quan ly HoiVien
    bool addHoiVien(HoiVien*);
    bool removeHoiVien(const string&);
    HoiVien* getHoiVien(const string&);
    const HoiVien* getHoiVien(const string&) const;

    // Ham quan ly HLV
    bool addHLV(HLV*);
    bool removeHLV(const string&);
    HLV* getHLV(const string&);
    const HLV* getHLV(const string&) const;

    // Ham quan ly NhanVien
    bool addNhanVien(NhanVien*);
    bool removeNhanVien(const string&);
    NhanVien* getNhanVien(const string&);
    const NhanVien* getNhanVien(const string&) const;

    // Ham quan ly GoiTap
    bool addGoiTap(GoiTap*);
    bool removeGoiTap(const string&);
    GoiTap* getGoiTap(const string&);
    const GoiTap* getGoiTap(const string&) const;
    
    // Ham quan ly LopHoc
    bool addLopHoc(LopHoc*);
    bool removeLopHoc(const string&);
    LopHoc* getLopHoc(const string&);
    const LopHoc* getLopHoc(const string&) const;

    // Ham quan ly MonTap
    bool addMonTap(MonTap*);
    bool removeMonTap(const string&);
    MonTap* getMonTap(const string&);
    const MonTap* getMonTap(const string&) const;

    // Ham quan ly HopDong
    bool addHopDong(HopDong*);
    bool cancelHopDong(const string&);
    HopDong* getHopDong(const string&);
    const HopDong* getHopDong(const string&) const;

    // Ham quan ly HangHoa
    bool addHangHoa(HangHoa*);
    bool removeHangHoa(const string&);
    HangHoa* getHangHoa(const string&);
    const HangHoa* getHangHoa(const string&) const;

    // Ham quan ly HoaDon
    bool addHoaDon(HoaDon*);
    HoaDon* getHoaDon(const string&);
    const HoaDon* getHoaDon(const string&) const;

    string getDisplayList(const string& entityName) const;
};

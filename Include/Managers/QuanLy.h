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
class LogTapPT;
class FileIO;

class QuanLy {
    friend class FileIO;
private:
    // Hang so kich thuoc hash table (so nguyen to de phan bo tot hon)
    static const size_t HOIVIEN_CAPACITY = 7013;    // So nguyen to nho cho du lieu hoi vien
    static const size_t TRANSACTION_CAPACITY = 40009; // So nguyen to lon cho cac giao dich
    
    static QuanLy* instance;
    
    // Luu tru du lieu - MyHashTable cho cac entity can tra cuu nhanh theo ID
    MyHashTable<HoiVien*> dsHoiVien;      // Hoi vien (tra cuu thuong xuyen)
    MyHashTable<HopDong*> dsHopDong;      // Hop dong (tra cuu thuong xuyen)
    MyHashTable<HoaDon*> dsHoaDon;        // Hoa don (tra cuu thuong xuyen)
    MyHashTable<LogTapPT*> dsLogTapPT;    // Nhat ky tap PT (tra cuu thuong xuyen)
    
    // MyVector cho cac entity duyet tuan tu
    MyVector<HLV*> dsHLV;                 // Huan luyen vien
    MyVector<NhanVien*> dsNhanVien;       // Nhan vien
    MyVector<GoiTap*> dsGoiTap;           // Goi tap
    MyVector<LopHoc*> dsLopHoc;           // Lop hoc
    MyVector<MonTap*> dsMonTap;           // Mon tap
    MyVector<HangHoa*> dsHangHoa;         // Hang hoa

    bool isDirty = false;                 // Danh dau du lieu can luu

    QuanLy() : dsHoiVien(HOIVIEN_CAPACITY), 
               dsHoaDon(TRANSACTION_CAPACITY), 
               dsHopDong(TRANSACTION_CAPACITY), 
               dsLogTapPT(TRANSACTION_CAPACITY) {}
    QuanLy(const QuanLy&) = delete;
    QuanLy& operator=(const QuanLy&) = delete;

public:
    static QuanLy& getInstance() {
        static QuanLy instance;
        return instance;
    }

    ~QuanLy();

    void setDirty(bool status = true);
    bool getIsDirty() const;    

    // Ham quan ly HoiVien
    bool addHoiVien(HoiVien*);
    bool removeHoiVien(const string&);
    HoiVien* getHoiVien(const string&);
    HoiVien* getHoiVienBySDT(const string&);
    const HoiVien* getHoiVien(const string&) const;

    // Ham quan ly HLV
    bool addHLV(HLV*);
    bool removeHLV(const string&);
    HLV* getHLV(const string&);
    HLV* getHLVBySDT(const string&);
    const HLV* getHLV(const string&) const;

    // Ham quan ly NhanVien
    bool addNhanVien(NhanVien*);
    bool removeNhanVien(const string&);
    NhanVien* getNhanVien(const string&);
    NhanVien* getNhanVienBySDT(const string&);
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

    // Ham quan ly LogTapPT
    bool addLogTapPT(LogTapPT*);
    LogTapPT* getLogTapPT(const string&);
    const LogTapPT* getLogTapPT(const string&) const;

    string getDisplayList(const string& entityName) const;
};

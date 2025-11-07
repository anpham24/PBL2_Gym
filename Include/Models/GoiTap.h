#pragma once
#include "MyVector.h"
#include <string>
using namespace std;

class MonTap;
class HopDong;
class ChiTietHoaDon_GT;

class GoiTap {
private:
    string id;
    string tenGoi;  
    int thoiGian; // số tháng
    int soBuoiPT; // số buổi tập PT
    double gia; // giá niêm yết
    bool isActive;
    MyVector<MonTap*> dsMonTap;
    MyVector<HopDong*> dsHopDong;
    MyVector<ChiTietHoaDon_GT*> dsChiTietHoaDon_GT;
public:
    GoiTap();
    GoiTap(const string&, const string&, int, int, double, bool = true);
    GoiTap(const GoiTap& other);
    ~GoiTap();

    const string& getID() const;
    const string& getTenGoi() const;
    int getThoiGian() const;
    int getSoBuoiPT() const;
    double getGia() const;
    bool getIsActive() const;

    void setTenGoi(const string&);
    void setThoiGian(int);
    void setSoBuoiPT(int);
    void setGia(double);
    void setIsActive(bool);

    void addMonTap(MonTap*);
    void removeMonTap(MonTap*);
    const MyVector<MonTap*>& getDsMonTap() const;
    MyVector<MonTap*>& getDsMonTap();

    void addHopDong(HopDong*);
    void removeHopDong(HopDong*);
    const MyVector<HopDong*>& getDsHopDong() const;
    MyVector<HopDong*>& getDsHopDong();

    void addChiTietHoaDon_GT(ChiTietHoaDon_GT*);
    void removeChiTietHoaDon_GT(ChiTietHoaDon_GT*);
    const MyVector<ChiTietHoaDon_GT*>& getDsChiTietHoaDon_GT() const;
    MyVector<ChiTietHoaDon_GT*>& getDsChiTietHoaDon_GT();

    static GoiTap* create(const string&, const string&, int, int, double, bool = true);
    static GoiTap* create(const string&, int, int, double, bool = true);
    string read() const;
};
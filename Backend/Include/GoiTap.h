#pragma once
#include "MyVector.h"
#include <string>
using namespace std;

class MonTap;
class ChiTietHoaDon_GT;

class GoiTap {
private:
    string id;
    string tenGoi;  
    int thoiGian; // số tháng
    double gia; // giá niêm yết
    MyVector<MonTap*> dsMonTap;
    MyVector<ChiTietHoaDon_GT*> dsChiTietHoaDon_GT;
public:
    GoiTap();
    GoiTap(const string&, int, double);
    GoiTap(const GoiTap& other);
    ~GoiTap();

    const string& getID() const;
    const string& getTenGoi() const;
    int getThoiGian() const;
    double getGia() const;

    void setTenGoi(const string&);
    void setThoiGian(int);
    void setGia(double);

    void addMonTap(MonTap*);
    void removeMonTap(MonTap*);
    const MyVector<MonTap*>& getDsMonTap() const;
    MyVector<MonTap*>& getDsMonTap();

    void addChiTietHoaDon_GT(ChiTietHoaDon_GT*);
    void removeChiTietHoaDon_GT(ChiTietHoaDon_GT*);
    const MyVector<ChiTietHoaDon_GT*>& getDsChiTietHoaDon_GT() const;
    MyVector<ChiTietHoaDon_GT*>& getDsChiTietHoaDon_GT();

    static GoiTap* create(const string&, int, double);
    string read() const;
};
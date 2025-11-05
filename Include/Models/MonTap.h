#pragma once
#include "MyVector.h"
#include <string>
using namespace std;

class LopHoc;
class GoiTap;

class MonTap {
private:
    string id;
    string tenMon;
    MyVector<LopHoc*> dsLopHoc;
    MyVector<GoiTap*> dsGoiTap;
public:
    MonTap();
    MonTap(const string&, const string&);
    MonTap(const MonTap& other);
    ~MonTap();

    const string& getID() const;
    const string& getTenMon() const;

    void setTenMon(const string&);

    void addLopHoc(LopHoc*);
    void removeLopHoc(LopHoc*);
    const MyVector<LopHoc*>& getDsLopHoc() const;
    MyVector<LopHoc*>& getDsLopHoc();

    void addGoiTap(GoiTap*);
    void removeGoiTap(GoiTap*);
    const MyVector<GoiTap*>& getDsGoiTap() const;
    MyVector<GoiTap*>& getDsGoiTap();

    static MonTap* create(const string&, const string&);
    static MonTap* create(const string&);
    string read() const;
};
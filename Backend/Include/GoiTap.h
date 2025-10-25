#pragma once
#include <string>
#include "MyVector.h"
using namespace std;

class GoiTap {
private:
    string id;
    string tenGoi;  
    int thoiGian; // số tháng
    double gia; // giá niêm yết
    string lichTap;
    MyVector<string> maMon;
public:
    GoiTap();
    GoiTap(const string&, int, double, const string&);
    GoiTap(const GoiTap& other);
    ~GoiTap();

    const string& getID() const;
    const string& getTenGoi() const;
    int getThoiGian() const;
    double getGia() const;
    const string& getLichTap() const;
    const MyVector<string>& getMaMon() const;
    const string& getMaHLV() const;

    void setTenGoi(const string&);
    void setThoiGian(int);
    void setGia(double);
    void setLichTap(const string&);
    void addMaMon(const string&);
    void setMaHLV(const string&);

    static GoiTap create(const string&, int, double, const string&);
    void update(const string&, int, double, const string&);
    string read() const;
};
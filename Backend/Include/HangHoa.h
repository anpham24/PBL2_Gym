#pragma once
#include <string>
using namespace std;

class HangHoa {
private:
    string id;
    string tenHH;
    double gia;
    int soLuongCon;
public:
    HangHoa();
    HangHoa(const string&, double, int);
    HangHoa(const HangHoa& other);
    ~HangHoa();

    const string& getID() const;
    const string& getTenHH() const;
    double getGia() const;
    int getSoLuongCon() const;

    void setTenHH(const string&);
    void setGia(double);
    void setSoLuongCon(int);

    static HangHoa* create(const string&, double, int);
    string read() const;
};
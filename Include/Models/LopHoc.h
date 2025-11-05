#pragma once
#include <string>
using namespace std;

class MonTap; // Forward declaration
class HLV;   // Forward declaration

class LopHoc {
private:
    string id;
    string tenLop;
    string lichTap;
    int thoiLuong; // in minutes
    MonTap* monTap; // Pointer to associated MonTap
    HLV* hlv;     // Pointer to associated HLV
public: 
    LopHoc();
    LopHoc(const string&, const string&, int, MonTap*, HLV*);
    LopHoc(const LopHoc& other);
    ~LopHoc();

    string getID() const;
    string getTenLop() const;
    string getLichTap() const;
    int getThoiLuong() const;
    MonTap* getMonTap() const;
    HLV* getHLV() const;

    void setTenLop(const string&);
    void setLichTap(const string&);
    void setThoiLuong(int);
    void setMonTap(MonTap*);
    void setHLV(HLV*);

    static LopHoc* create(const string&, const string&, const string&, int, MonTap*, HLV*);
    string read() const;
};
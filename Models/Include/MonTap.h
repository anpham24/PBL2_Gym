#include "MyVector.h"
#include <string>
using namespace std;

class LopHoc;
class GoiTap;

class MonTap {
private:
    string id;
    string tenMon;
    string lichTap;
    string maHLV;
    MyVector<LopHoc*> dsLopHoc;
    MyVector<GoiTap*> dsGoiTap;
public:
    MonTap();
    MonTap(const string&, const string&, const string& = "");
    MonTap(const MonTap& other);
    ~MonTap();

    const string& getID() const;
    const string& getTenMon() const;
    const string& getLichTap() const;
    const string& getMaHLV() const;

    void setTenMon(const string&);
    void setLichTap(const string&);
    void setMaHLV(const string&);

    void addLopHoc(LopHoc*);
    void removeLopHoc(LopHoc*);
    const MyVector<LopHoc*>& getDsLopHoc() const;
    MyVector<LopHoc*>& getDsLopHoc();

    void addGoiTap(GoiTap*);
    void removeGoiTap(GoiTap*);
    const MyVector<GoiTap*>& getDsGoiTap() const;
    MyVector<GoiTap*>& getDsGoiTap();

    static MonTap* create(const string&, const string&, const string& = "");
    string read() const;
};
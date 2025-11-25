#pragma once
#include "Person.h"
#include "MyVector.h"

class LopHoc;
class LogTapPT;

class HLV : public Person {
private:
    string chuyenMon;
    double luongCoBan; 
    bool isActive;
    MyVector<LopHoc*> dsLopHoc;
    MyVector<LogTapPT*> dsLogTapPT;
    
public:
    HLV();
    HLV(const string&, const string&, const string&, const string&, const string&, 
        const string&, double = 0.0, bool = true);
    HLV(const HLV&);
    ~HLV();

    // Getters
    string getChuyenMon() const;
    double getLuongCoBan() const;              
    double getLuong() const;                   
    int getSoBuoiPT() const;                   
    int getTongSoBuoiLop() const;              
    bool getIsActive() const;

    // Setters
    void setChuyenMon(const string&);
    void setLuongCoBan(double);                
    void setIsActive(bool);

    void addLogTapPT(LogTapPT*);
    void removeLogTapPT(LogTapPT*);
    const MyVector<LogTapPT*>& getDsLogTapPT() const;
    MyVector<LogTapPT*>& getDsLogTapPT();

    void addLopHoc(LopHoc*);
    void removeLopHoc(LopHoc*);
    const MyVector<LopHoc*>& getDsLopHoc() const;
    MyVector<LopHoc*>& getDsLopHoc();

    static HLV* create(const string&, const string&, const string&, const string&, 
                      const string&, const string&, double = 0.0, bool = true);
    static HLV* create(const string&, const string&, const string&, const string&, 
                      const string&, double = 0.0, bool = true);
    string read() const override;
};
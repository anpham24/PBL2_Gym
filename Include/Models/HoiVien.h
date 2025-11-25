#pragma once
#include "Person.h"
#include "MyVector.h"
#include "Config.h"

class HoaDon; // Forward declaration
class HopDong; // Forward declaration
class LogTapPT; // Forward declaration

class HoiVien : public Person {
private:
    int point;
    int soBuoiPT;
    bool isActive;
    MyVector<HoaDon*> dsHoaDon;
    MyVector<HopDong*> dsHopDong;
    MyVector<LogTapPT*> dsLogTapPT;
public:
    HoiVien();
    HoiVien(const string&, const string&, const string&, const string&, 
            const string&, int = 0, int = 0, bool = true);
    HoiVien(const HoiVien& other);
    ~HoiVien();

    // Getters
    int getPoint() const;
    int getSoBuoiPT() const;
    bool getIsActive() const;
    string getRank() const;
    Config::Rank getRankEnum() const; // ✅ THÊM - Trả về enum
    double getDiscountRate() const;   // ✅ THÊM - Lấy % giảm giá

    // Setters
    void setPoint(int);
    void setSoBuoiPT(int);
    void setIsActive(bool);

    void addHoaDon(HoaDon*);
    void removeHoaDon(HoaDon*);
    const MyVector<HoaDon*>& getDsHoaDon() const;
    MyVector<HoaDon*>& getDsHoaDon();

    void addHopDong(HopDong*);
    void removeHopDong(HopDong*);
    const MyVector<HopDong*>& getDsHopDong() const;
    MyVector<HopDong*>& getDsHopDong();

    void addLogTapPT(LogTapPT*);
    void removeLogTapPT(LogTapPT*);
    const MyVector<LogTapPT*>& getDsLogTapPT() const;
    MyVector<LogTapPT*>& getDsLogTapPT();

    static HoiVien* create(const string&, const string&, const string&, const string&, 
                            const string&, int = 0, int = 0, bool = true);
    static HoiVien* create(const string&, const string&, const string&, 
                            const string&, int = 0, int = 0, bool = true);
    string read() const override;
};
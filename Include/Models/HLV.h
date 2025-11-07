#pragma once
#include "Person.h"
#include "MyVector.h"

class LopHoc; // Forward declaration
class LogTapPT; // Forward declaration

class HLV : public Person {
    private:
        string chuyenMon;
        double luong;
        bool isActive;
        MyVector<LopHoc*> dsLopHoc;
        MyVector<LogTapPT*> dsLogTapPT;
    public:
        HLV();
        HLV(const string&, const string&, const string&, const string&, const string&, const string&, double = 0.0, bool = true);
        HLV(const HLV&);
        ~HLV();

        string getChuyenMon() const;
        void setChuyenMon(const string&);
        double getLuong() const;
        void setLuong(double);
        bool getIsActive() const;
        void setIsActive(bool);

        void addLogTapPT(LogTapPT*);
        void removeLogTapPT(LogTapPT*);
        const MyVector<LogTapPT*>& getDsLogTapPT() const;
        MyVector<LogTapPT*>& getDsLogTapPT();

        void addLopHoc(LopHoc*);
        void removeLopHoc(LopHoc*);
        const MyVector<LopHoc*>& getDsLopHoc() const;
        MyVector<LopHoc*>& getDsLopHoc();

        static HLV* create(const string&, const string&, const string&, const string&, const string&, const string&, double = 0.0, bool = true);
        static HLV* create(const string&, const string&, const string&, const string&, const string&, double = 0.0, bool = true);
        string read() const override;
};
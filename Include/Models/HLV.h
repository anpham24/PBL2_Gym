#pragma once
#include "Person.h"
#include "MyVector.h"

class HoiVien; // Forward declaration
class LopHoc; // Forward declaration

class HLV : public Person {
    private:
        double luong;
        MyVector<HoiVien*> dsHoiVien;
        MyVector<LopHoc*> dsLopHoc;
    public:
        HLV();
        HLV(const string&, const string&, const string&, const string&, int, double = 0.0);
        HLV(const HLV& other);
        ~HLV();

        double getLuong() const;
        void setLuong(double);

        void addHoiVien(HoiVien*);
        void removeHoiVien(HoiVien*);
        const MyVector<HoiVien*>& getDsHoiVien() const;
        MyVector<HoiVien*>& getDsHoiVien();

        void addLopHoc(LopHoc*);
        void removeLopHoc(LopHoc*);
        const MyVector<LopHoc*>& getDsLopHoc() const;
        MyVector<LopHoc*>& getDsLopHoc();

        static HLV* create(const string&, const string&, const string&, const string&, int, double = 0.0);
        static HLV* create(const string&, const string&, const string&, int, double = 0.0);
        string read() const override;
};
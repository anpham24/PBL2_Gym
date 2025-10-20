#pragma once
#include "Person.h"

class HoiVien : public Person {
    private:
        int point;
        string maHLV;
    public:
        HoiVien();
        HoiVien(const string&, const string&, const string&, int = 0, const string& = "");
        ~HoiVien();

        double getPoint() const;
        void setPoint(int);

        static HoiVien create(const string&, const string&, const string&, int = 0, const string& = "");
        string read() const override;
        void update(const string&, const string&, const string&, int, const string&);
};
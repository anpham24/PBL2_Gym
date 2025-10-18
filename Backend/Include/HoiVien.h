#pragma once
#include "Person.h"

class HoiVien : public Person {
    private:
        int point;
    public:
        HoiVien();
        HoiVien(const string&, const string&, const string&, int = 0);
        ~HoiVien();

        double getPoint() const;
        void setPoint(int);

        static HoiVien create(const string&, const string&, const string&, int);
        string read() const override;
        void update(const string&, const string&, const string&, int);
};
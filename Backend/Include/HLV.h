#pragma once
#include "Person.h"

class HLV : public Person {
    private:
        double luong;
    public:
        HLV();
        HLV(const string&, const string&, const string&, int, double = 0.0);
        HLV(const HLV& other);
        ~HLV();

        double getLuong() const;
        void setLuong(double);

        static HLV create(const string&, const string&, const string&, int, double);
        string read() const override;
        void update(const string&, const string&, const string&, int, double);
};
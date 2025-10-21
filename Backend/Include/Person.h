#pragma once
#include <string>
using namespace std;

class Person {
    protected:
        string id;
        string hoTen;
        string sdt;
        string gioiTinh;
    public:
        Person();
        Person(const string&, const string&, const string&);
        virtual ~Person() = 0;

        const string& getID() const;
        const string& getHoTen() const;
        const string& getSDT() const;
        const string& getGioiTinh() const;

        void setHoTen(const string&);
        void setSDT(const string&);
        void setGioiTinh(const string&);

        virtual string read() const = 0;
};
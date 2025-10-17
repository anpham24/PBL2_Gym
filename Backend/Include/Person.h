#pragma once
#include <iostream>
#include <string>
#include <map>
using namespace std;

class Person {
    protected:
        string ID;
        string hoTen;
        string sdt;
        string gioiTinh;
        static map<int, int> nextIDMap;
        static int lastYear;
        static string generateID(int prefix);
    public:
        Person();
        Person(const string&, const string&, const string&);
        virtual ~Person() = 0;

        string getID() const;
        string getHoTen() const;
        string getSDT() const;
        string getGioiTinh() const;

        void setID(const string&);
        void setHoTen(const string&);
        void setSDT(const string&);
        void setGioiTinh(const string&);

        virtual string read() const = 0;
};
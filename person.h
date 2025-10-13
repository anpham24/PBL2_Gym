#include <iostream>
#include <string>
using namespace std;

class Person {
    protected:
        string ID;
        string hoTen;
        string sdt;
        string gioiTinh;
    public:
        Person();
        Person(const string&, const string&, const string&, const string&);
        virtual ~Person() = 0;

        string getID() const;
        string getHoTen() const;
        string getSDT() const;
        string getGioiTinh() const;

        void setID(const string&);
        void setHoTen(const string&);
        void setSDT(const string&);
        void setGioiTinh(const string&);

        virtual void create() = 0;
        virtual void read() const = 0;
        virtual void update() = 0;
        virtual void remove() = 0;

};
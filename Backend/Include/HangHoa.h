#pragma once
#include <string>
using namespace std;

class HangHoa {
    private:
        string id;
        string tenHH;
        double gia;
    public:
        HangHoa();
        HangHoa(const string&, double);
        HangHoa(const HangHoa& other);
        ~HangHoa();

        const string& getID() const;
        const string& getTenHH() const;
        double getGia() const;

        void setTenHH(const string&);
        void setGia(double);

        static HangHoa* create(const string&, double);
        string read() const;
};
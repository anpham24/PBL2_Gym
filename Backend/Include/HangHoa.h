#pragma once
#include <string>
using namespace std;

class HangHoa {
    private:
        string id;
        string tenHH;
        double gia; // giá niêm yết
    public:
        HangHoa();
        HangHoa(const string&, double);
        ~HangHoa();

        string getID() const;
        string getTenHH() const;
        double getGia() const;

        void setTenHH(const string&);
        void setGia(double);

        static HangHoa create(const string&, double);
        void update(const string&, double);
        string read() const;
};
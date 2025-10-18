#pragma once
#include <string>
using namespace std;

class GoiTap {
    private:
        string id;
        string tenGoi;  
        int thoiGian; // số tháng
        double gia; // giá niêm yết
        string lichTap;
        string maMon;
        string maHLV;
    public:
        GoiTap();
        GoiTap(const string&, int, double, const string&, const string&, const string&);
        ~GoiTap();

        string getID() const;
        string getTenGoi() const;
        int getThoiGian() const;
        double getGia() const;
        string getLichTap() const;
        string getMaMon() const;
        string getMaHLV() const;

        void setTenGoi(const string&);
        void setThoiGian(int);
        void setGia(double);
        void setLichTap(const string&);
        void setMaMon(const string&);
        void setMaHLV(const string&);

        static GoiTap create(const string&, int, double, const string&, const string&, const string&);
        void update(const string&, int, double, const string&, const string&, const string&);
        string read() const;
};
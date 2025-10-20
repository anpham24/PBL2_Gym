#pragma once
#include <string>
#include <vector>
#include "../Include/ChiTietHD.h"
using namespace std;

class HoaDon {
    private:
        string id;
        string maHV;
        string maNV;
        string ngayLap;
        string phuongThucTT;
        vector<ChiTietHD> items;
    public:
        HoaDon();
        HoaDon(const string&, const string&, const string&, const string&);
        ~HoaDon();

        const string& getID() const;
        const string& getMaNV() const;
        const string& getNgayLap() const;
        const string& getPhuongThuc() const;
        const vector<ChiTietHD>& getItems() const;

        void setMaNV(const string &maNV);
        void setNgayLap(const string &ngayLap);
        void setPhuongThuc(const string &pt);
        void setGhiChu(const string &gc);

        
};
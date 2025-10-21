#pragma once
#include "ChiTietHD.h"
#include <string>
#include <vector>

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
        const string& getMaHV() const;
        const string& getMaNV() const;
        const string& getNgayLap() const;
        const string& getPhuongThuc() const;
        const vector<ChiTietHD>& getItems() const;

        void setMaHV(const string&);
        void setMaNV(const string&);
        void setNgayLap(const string&);
        void setPhuongThucTT(const string&);
        
        void addItem(const ChiTietHD&);
        bool removeItemByMaSP(const string&);

        double getTotal() const;
        size_t itemCount() const;
};
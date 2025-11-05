#include "FileIO.h"
#include "QuanLy.h"
#include "StringUtils.h"
#include "HoiVien.h"
#include "HLV.h"
#include "NhanVien.h"
#include "GoiTap.h"
#include "LopHoc.h"
#include "MonTap.h"
#include "HopDong.h"
#include "HangHoa.h"
#include "HoaDon.h"
#include "ChiTietHoaDon_GT.h"
#include "ChiTietHoaDon_HH.h"
#include <fstream>

void FileIO::loadNhanVien(QuanLy& ql, const string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Goi ham tien ich de tach chuoi
        MyVector<std::string> parts = splitString(line, '|');

        // Kiem tra xem co du so luong phan tu khong (vi du: 7 phan)
        if (parts.size() < 7) continue; // Bo qua dong loi

        // Gan vao bien
        string maNV = parts[0];
        string tenNV = parts[1];
        string sdt = parts[2];
        string gioiTinh = parts[3];
        int tuoi = stoi(parts[4]);
        double luong = stod(parts[5]);
        bool isActive = toBool(parts[6]);

        NhanVien* nv = NhanVien::create(maNV, tenNV, sdt, gioiTinh, tuoi, luong, isActive);
        ql.addNhanVien(nv);
    }
    file.close();
}

void FileIO::loadHLV(QuanLy& ql, const string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        MyVector<std::string> parts = splitString(line, '|');

        if (parts.size() < 6) continue;

        string maHLV = parts[0];
        string tenHLV = parts[1];
        string sdt = parts[2];
        string gioiTinh = parts[3];
        int tuoi = stoi(parts[4]);
        double luong = stod(parts[5]);

        HLV* hlv = HLV::create(maHLV, tenHLV, sdt, gioiTinh, tuoi, luong);
        ql.addHLV(hlv);
    }
    file.close();
}

void FileIO::loadMonTap(QuanLy& ql, const string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        MyVector<std::string> parts = splitString(line, '|');

        if (parts.size() < 2) continue;

        string maMT = parts[0];
        string tenMon = parts[1];

        MonTap* mt = MonTap::create(maMT, tenMon);
        ql.addMonTap(mt);
    }
    file.close();
}

void FileIO::loadHangHoa(QuanLy& ql, const string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;
    string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        MyVector<std::string> parts = splitString(line, '|');

        if (parts.size() < 5) continue;

        string maHH = parts[0];
        string tenHH = parts[1];
        double gia = stod(parts[2]);
        int soLuongCon = stoi(parts[3]);
        bool isActive = toBool(parts[4]);

        HangHoa* hh = HangHoa::create(maHH, tenHH, gia, soLuongCon, isActive);
        ql.addHangHoa(hh);
    }
    file.close();
}

void FileIO::loadHoiVien(QuanLy& ql, const string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        MyVector<std::string> parts = splitString(line, '|');
        
        // Cấu trúc: <MaHV>|<TenHV>|<SDT>|<GioiTinh>|<Point>|<isActive>|<MaHLV_FK>
        if (parts.size() < 7) continue; 

        string maHV = parts[0];
        string tenHV = parts[1];
        string sdt = parts[2];
        string gioiTinh = parts[3];
        int tuoi = stoi(parts[4]);
        int point = stoi(parts[5]);
        bool isActive = toBool(parts[6]);
        string maHLV_FK = parts[7];

        HoiVien* hv = HoiVien::create(maHV, tenHV, sdt, gioiTinh, tuoi, point, nullptr, isActive);
        
        HLV* hlv = ql.getHLV(maHLV_FK);
        hv->setHLV(hlv);

        ql.addHoiVien(hv);
    }
    file.close();
}
/**
 * FileIO.cpp - Cac thao tac nhap/xuat file cho he thong quan ly phong gym
 * 
 * File nay xu ly viec load va save tat ca du lieu entity tu/vao cac file CSV.
 * Moi loai entity co file rieng voi dinh dang cu the (phan cach bang dau cham phay).
 * 
 * Thu tu load quan trong doi voi cac entity co su phu thuoc:
 * 1. Cac entity doc lap (NhanVien, HLV, MonTap, GoiTap, HangHoa, HoiVien)
 * 2. Cac entity co tham chieu don gian (LopHoc can MonTap+HLV, HopDong can HoiVien+GoiTap)
 * 3. Cac entity giao dich (HoaDon can NhanVien+HoiVien)
 * 4. Cac entity chi tiet (ChiTietHoaDon_* can HoaDon va HangHoa/GoiTap)
 * 5. Cac entity log (LogTapPT can HoiVien+HLV)
 */

#include "FileIO.h"
#include "QuanLy.h"
#include "StringUtils.h"
#include "AccountManager.h"
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
#include "LogTapPT.h"
#include "IDGenerator.h"
#include "SearchService.h"
#include <fstream>
#include <iostream>

// ============================================================================
// CAC HAM LOAD - Doc file CSV va dien du lieu vao cau truc QuanLy
// ============================================================================

void FileIO::loadNhanVien(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Goi ham tien ich de tach chuoi
        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaNV>;<TenNV>;<SDT>;<GioiTinh>;<NgaySinh>;<Luong>;<isActive>
        // Kiem tra xem co du so luong phan tu khong
        if (parts.size() < 7) continue; // Bo qua dong loi

        // Gan vao bien
        string maNV = parts[0];
        string tenNV = parts[1];
        string sdt = parts[2];
        string gioiTinh = parts[3];
        string ngaySinh = parts[4];
        double luong = stod(parts[5]);
        bool isActive = toBool(parts[6]);

        NhanVien* nv = NhanVien::create(maNV, tenNV, sdt, gioiTinh, ngaySinh, luong, isActive);
        ql.addNhanVien(nv);
    }
    file.close();
}

void FileIO::loadHLV(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaHLV>;<TenHLV>;<SDT>;<GioiTinh>;<NgaySinh>;<Luong>;<isActive>
        if (parts.size() < 7) continue;

        string maHLV = parts[0];
        string tenHLV = parts[1];
        string sdt = parts[2];
        string gioiTinh = parts[3];
        string ngaySinh = parts[4];
       string chuyenMon = parts[5]; // ✅ THÊM DÒNG NÀY!
            double luong = stod(parts[6]);
            bool isActive = toBool(parts[7]);

        HLV* hlv = HLV::create(maHLV, tenHLV, sdt, gioiTinh, ngaySinh, chuyenMon, luong, isActive);
        ql.addHLV(hlv);
    }
    file.close();
}

void FileIO::loadHoiVien(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaHV>;<TenHV>;<SDT>;<GioiTinh>;<NgaySinh>;<Point>;<SoBuoiPT>;<isActive>
        if (parts.size() < 8) continue;

        string maHV = parts[0];
        string tenHV = parts[1];
        string sdt = parts[2];
        string gioiTinh = parts[3];
        string ngaySinh = parts[4];
        int point = stoi(parts[5]);
        int soBuoiPT = stoi(parts[6]);
        bool isActive = toBool(parts[7]);

        HoiVien* hv = HoiVien::create(maHV, tenHV, sdt, gioiTinh, ngaySinh, point, soBuoiPT, isActive);
        ql.addHoiVien(hv);
    }
    file.close();
}

void FileIO::loadMonTap(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        if (parts.size() < 2) continue;

        string maMT = parts[0];
        string tenMon = parts[1];

        MonTap* mt = MonTap::create(maMT, tenMon);
        ql.addMonTap(mt);
    }
    file.close();
}

void FileIO::loadGoiTap(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaGoi>;<TenGoi>;<ThoiGian>;<SoBuoiPT>;<Gia>;<isActive>
        if (parts.size() < 6) continue;

        string maGoi = parts[0];
        string tenGoi = parts[1];
        int thoiGian = stoi(parts[2]);
        int soBuoiPT = stoi(parts[3]);
        double gia = stod(parts[4]);
        bool isActive = toBool(parts[5]);

        GoiTap* gt = GoiTap::create(maGoi, tenGoi, thoiGian, soBuoiPT, gia, isActive);
        ql.addGoiTap(gt);
    }
    file.close();
}

void FileIO::loadHangHoa(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');
        
        // Cấu trúc: <MaHH>;<TenHH>;<LoaiHH>;<Gia>;<SoLuongCon>;<isActive>
        if (parts.size() < 6) continue;

        string maHH = parts[0];
        string tenHH = parts[1];
        string loaiHH = parts[2];
        double gia = stod(parts[3]);
        int soLuongCon = stoi(parts[4]);
        bool isActive = toBool(parts[5]);

        HangHoa* hh = HangHoa::create(maHH, tenHH, loaiHH, gia, soLuongCon, isActive);
        ql.addHangHoa(hh);
    }
    file.close();
}

void FileIO::loadLopHoc(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaLop>;<TenLop>;<LichHoc>;<ThoiLuong>;<MaMonTap_FK>;<MaHLV_FK>
        if (parts.size() < 4) continue;

        string maLop = parts[0];
        string tenLop = parts[1];
        string lichHoc = parts[2];
        int thoiLuong = stoi(parts[3]);

        LopHoc* lh = LopHoc::create(maLop, tenLop, lichHoc, thoiLuong); // Chi tao LopHoc truoc, chua tao lien ket den MonTap va HLV
        ql.addLopHoc(lh);
    }
    file.close();
}

void FileIO::loadHopDong(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaHD>;<NgayDK>;<NgayHetHan>;<isActive>;<MaHV_FK>;<MaGoiTap_FK>;<MaNV_FK>
        if (parts.size() < 4) continue;

        string maHD = parts[0];
        string ngayDK = parts[1];
        string ngayHetHan = parts[2];
        bool isActive = toBool(parts[3]);
        int soBuoiPT = stoi(parts[4]); // ✅ THÊM

        HopDong* hd = HopDong::create(maHD, ngayDK, ngayHetHan, isActive, soBuoiPT);
        ql.addHopDong(hd);
    }
    file.close();
}

void FileIO::loadHoaDon(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // ✅ FORMAT MỚI: <MaHD>;<NgayLap>;<PhuongThucTT>;<GiamGia>;<NV_FK>;<HV_FK>
        if (parts.size() < 4) continue; // Tối thiểu 4 trường

        string maHD = parts[0];
        string ngayLap = parts[1];
        string phuongThucTT = parts[2];
        double giamGia = stod(parts[3]);

        HoaDon* hd = HoaDon::create(maHD, ngayLap, phuongThucTT);
        hd->setGiamGia(giamGia);
        
        // ✅ THÊM: Đánh dấu đã thanh toán (vì đã lưu vào file)
        hd->setDaThanhToan(true);
        
        ql.addHoaDon(hd);
    }
    file.close();
}

void FileIO::loadLogTapPT(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaLog>;<NgayGio>;<MaHV_FK>;<MaHLV_FK>
        if (parts.size() < 4) continue;

        string maLog = parts[0];
        string ngayGio = parts[1];

        LogTapPT* log = LogTapPT::create(maLog, ngayGio); // Chua lien ket den HoiVien va HLV
        ql.addLogTapPT(log);
    }
    file.close();
}

void FileIO::loadChiTietHoaDon_GT(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaHD_FK>;<MaGoiTap_FK>;<SoLuong>;<DonGia>
        if (parts.size() < 4) continue;

        string maHD = parts[0];
        string maGoiTap = parts[1];
        int soLuong = stoi(parts[2]);
        double donGia = stod(parts[3]);

        HoaDon* hd = ql.getHoaDon(maHD);
        GoiTap* gt = ql.getGoiTap(maGoiTap);
        if (hd && gt) {
            ChiTietHoaDon_GT* ct = ChiTietHoaDon_GT::create(soLuong, donGia, hd, gt);
            hd->addChiTietHoaDon_GT(ct);
            gt->addChiTietHoaDon_GT(ct);
        }
    }
    file.close();
}   

void FileIO::loadChiTietHoaDon_HH(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaHD_FK>;<MaHangHoa_FK>;<SoLuong>;<DonGia>
        if (parts.size() < 4) continue;

        string maHD = parts[0];
        string maHangHoa = parts[1];
        int soLuong = stoi(parts[2]);
        double donGia = stod(parts[3]);

        HoaDon* hd = ql.getHoaDon(maHD);
        HangHoa* hh = ql.getHangHoa(maHangHoa);
        if (hd && hh) {
            ChiTietHoaDon_HH* ct = ChiTietHoaDon_HH::create(soLuong, donGia, hd, hh);
            hd->addChiTietHoaDon_HH(ct);
            hh->addChiTietHoaDon_HH(ct);
        }
    }
    file.close();
}

void FileIO::linkLopHoc(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaLop>;<TenLop>;<LichHoc>;<ThoiLuong>;<MaMonTap_FK>;<MaHLV_FK>
        if (parts.size() < 6) continue;

        string maLop = parts[0];
        string maMonTap = parts[4];
        string maHLV = parts[5];

        LopHoc* lh = ql.getLopHoc(maLop);
        MonTap* mt = ql.getMonTap(maMonTap);
        HLV* hlv = ql.getHLV(maHLV);
        if (lh) {
            if (mt) lh->setMonTap(mt);
            if (hlv) lh->setHLV(hlv);
        }
    }
    file.close();
}

void FileIO::linkHopDong(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaHD>;<NgayDK>;<NgayHetHan>;<isActive>;<MaHV_FK>;<MaGoiTap_FK>;<MaNV_FK>
        if (parts.size() < 7) continue;

        string maHD = parts[0];
        string maHV = parts[5];
        string maGoiTap = parts[6];
        string maNV = parts[7];

        HopDong* hd = ql.getHopDong(maHD);
        HoiVien* hv = ql.getHoiVien(maHV);
        GoiTap* gt = ql.getGoiTap(maGoiTap);
        NhanVien* nv = ql.getNhanVien(maNV);
        if (hd) {
            if (hv) hd->setHoiVien(hv);
            if (gt) hd->setGoiTap(gt);
            if (nv) hd->setNhanVien(nv);
        }
    }
    file.close();
}

void FileIO::linkHoaDon(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // ✅ FORMAT MỚI: <MaHD>;<NgayLap>;<PhuongThucTT>;<GiamGia>;<NV_FK>;<HV_FK>
        if (parts.size() < 6) continue; // Cần 6 trường

        string maHD = parts[0];
        string maNV = parts[4];  // ✅ INDEX 4
        string maHV = parts[5];  // ✅ INDEX 5

        HoaDon* hd = ql.getHoaDon(maHD);
        if (hd) {
            // Cho phép maNV = "NULL" hoặc "ADMIN"
            if (maNV != "NULL" && maNV != "ADMIN") {
                NhanVien* nv = ql.getNhanVien(maNV);
                if (nv) hd->setNhanVien(nv);
            }
            
            HoiVien* hv = ql.getHoiVien(maHV);
            if (hv) hd->setHoiVien(hv);
        }
    }
    file.close();
}

void FileIO::linkLogTapPT(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaLog>;<NgayGio>;<MaHV_FK>;<MaHLV_FK>
        if (parts.size() < 4) continue;

        string maLog = parts[0];
        string maHV = parts[2];
        string maHLV = parts[3];

        LogTapPT* log = ql.getLogTapPT(maLog);
        HoiVien* hv = ql.getHoiVien(maHV);
        HLV* hlv = ql.getHLV(maHLV);
        if (log) {
            if (hv) log->setHoiVien(hv);
            if (hlv) log->setHLV(hlv);
        }
    }
    file.close();
}

void FileIO::linkGoiTap_MonTap(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ifstream file(filePath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, ';');

        // Cấu trúc: <MaGoiTap_FK>;<MaMonTap_FK>
        if (parts.size() < 2) continue;

        string maGoiTap = parts[0];
        string maMonTap = parts[1];

        GoiTap* gt = ql.getGoiTap(maGoiTap);
        MonTap* mt = ql.getMonTap(maMonTap);
        if (gt && mt) {
            gt->addMonTap(mt);
            mt->addGoiTap(gt);
        }
    }
    file.close();
}

void FileIO::loadAllData(QuanLy& ql, AccountManager& am, const string& folderPath) {
   

    IDGenerator::loadState();
    loadNhanVien(folderPath + "/NhanVien.txt");
    loadHLV(folderPath + "/HLV.txt");
    loadHoiVien(folderPath + "/HoiVien.txt");
    loadMonTap(folderPath + "/MonTap.txt");
    loadGoiTap(folderPath + "/GoiTap.txt");
    loadHangHoa(folderPath + "/HangHoa.txt");

    loadLopHoc(folderPath + "/LopHoc.txt");
    loadHopDong(folderPath + "/HopDong.txt");
    loadHoaDon(folderPath + "/HoaDon.txt");
    loadLogTapPT(folderPath + "/LogTapPT.txt");

    loadChiTietHoaDon_HH(folderPath + "/ChiTietHoaDon_HH.txt");
    loadChiTietHoaDon_GT(folderPath + "/ChiTietHoaDon_GT.txt");

    loadAccounts(am, ql, folderPath + "/accounts.txt");
    
    linkLopHoc(folderPath + "/LopHoc.txt");
    linkHopDong(folderPath + "/HopDong.txt");
    linkHoaDon(folderPath + "/HoaDon.txt");
    linkLogTapPT(folderPath + "/LogTapPT.txt");
    linkGoiTap_MonTap(folderPath + "/GoiTap_MonTap.txt");

    SearchService::indexAllHoiVien();
}

void FileIO::saveNhanVien(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    for (size_t i = 0; i < ql.dsNhanVien.size(); ++i) {
        NhanVien* nv = ql.dsNhanVien[i];
        file << nv->read() << endl;
    }
    file.close();
}

void FileIO::saveHLV(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    for (size_t i = 0; i < ql.dsHLV.size(); ++i) {
        HLV* hlv = ql.dsHLV[i];
        file << hlv->read() << endl;
    }
    file.close();
}

void FileIO::saveHoiVien(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    MyVector<HoiVien*> dsHV = ql.dsHoiVien.getAllValues();
    for (size_t i = 0; i < dsHV.size(); ++i) {
        HoiVien* hv = dsHV[i];
        file << hv->read() << endl;
    }
    file.close();
}

void FileIO::saveMonTap(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    for (size_t i = 0; i < ql.dsMonTap.size(); ++i) {
        MonTap* mt = ql.dsMonTap[i];
        file << mt->read() << endl;
    }
    file.close();
}

void FileIO::saveGoiTap(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    for (size_t i = 0; i < ql.dsGoiTap.size(); ++i) {
        GoiTap* gt = ql.dsGoiTap[i];
        file << gt->read() << endl;
    }
    file.close();
}

void FileIO::saveHangHoa(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    for (size_t i = 0; i < ql.dsHangHoa.size(); ++i) {
        HangHoa* hh = ql.dsHangHoa[i];
        file << hh->read() << endl;
    }
    file.close();
}

void FileIO::saveLopHoc(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    for (size_t i = 0; i < ql.dsLopHoc.size(); ++i) {
        LopHoc* lh = ql.dsLopHoc[i];
        file << lh->read() << endl;
    }
    file.close();
}

void FileIO::saveHopDong(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    MyVector<HopDong*> dsHD = ql.dsHopDong.getAllValues();
    for (size_t i = 0; i < dsHD.size(); ++i) {
        HopDong* hd = dsHD[i];
        file << hd->read() << endl;
    }
    file.close();
}

void FileIO::saveHoaDon(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    MyVector<HoaDon*> dsHD = ql.dsHoaDon.getAllValues();
    for (size_t i = 0; i < dsHD.size(); ++i) {
        HoaDon* hd = dsHD[i];
        file << hd->read() << endl;
    }
    file.close();
}

void FileIO::saveLogTapPT(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    MyVector<LogTapPT*> dsLog = ql.dsLogTapPT.getAllValues();
    for (size_t i = 0; i < dsLog.size(); ++i) {
        LogTapPT* log = dsLog[i];
        file << log->read() << endl;
    }
    file.close();
}

void FileIO::saveChiTietHoaDon_HH(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    // Duyệt tất cả HoaDon và lưu ChiTietHoaDon_HH
    MyVector<HoaDon*> dsHD = ql.getDsHoaDon().getAllValues();
    for (size_t i = 0; i < dsHD.size(); ++i) {
        HoaDon* hd = dsHD[i];
        const MyVector<ChiTietHoaDon_HH*>& dsChiTiet = hd->getDsChiTietHoaDon_HH();
        
        for (size_t j = 0; j < dsChiTiet.size(); ++j) {
            ChiTietHoaDon_HH* ct = dsChiTiet[j];
            if (ct != nullptr) {
                file << ct->read() << endl;
            }
        }
    }
    file.close();
}

void FileIO::saveChiTietHoaDon_GT(const string& filePath) {
    QuanLy& ql = QuanLy::getInstance();
    ofstream file(filePath);
    if (!file.is_open()) return;

    // Duyệt tất cả HoaDon và lưu ChiTietHoaDon_GT
    MyVector<HoaDon*> dsHD = ql.getDsHoaDon().getAllValues();
    for (size_t i = 0; i < dsHD.size(); ++i) {
        HoaDon* hd = dsHD[i];
        const MyVector<ChiTietHoaDon_GT*>& dsChiTiet = hd->getDsChiTietHoaDon_GT();
        
        for (size_t j = 0; j < dsChiTiet.size(); ++j) {
            ChiTietHoaDon_GT* ct = dsChiTiet[j];
            if (ct != nullptr) {
                file << ct->read() << endl;
            }
        }
    }
    file.close();
}

void FileIO::saveAllData(const QuanLy& ql, const AccountManager& am, const string& folderPath) {
    saveAccounts(am, folderPath + "/accounts.txt");

    saveNhanVien(folderPath + "/NhanVien.txt");
    saveHLV(folderPath + "/HLV.txt");
    saveHoiVien(folderPath + "/HoiVien.txt");
    saveMonTap(folderPath + "/MonTap.txt");
    saveGoiTap(folderPath + "/GoiTap.txt");
    saveHangHoa(folderPath + "/HangHoa.txt");

    saveLopHoc(folderPath + "/LopHoc.txt");
    saveHopDong(folderPath + "/HopDong.txt");
    saveHoaDon(folderPath + "/HoaDon.txt");
    saveLogTapPT(folderPath + "/LogTapPT.txt");

    saveChiTietHoaDon_HH(folderPath + "/ChiTietHoaDon_HH.txt");
    saveChiTietHoaDon_GT(folderPath + "/ChiTietHoaDon_GT.txt");

    IDGenerator::saveState();
}

void FileIO::saveAccounts(const AccountManager& am, const string& filePath) {
    ofstream file(filePath);
    if (!file.is_open()) {
        // std::cerr << "Loi: Khong the mo file de luu Accounts: " << filePath << std::endl;
        return;
    }

    const MyVector<Account*>& ds = am.getDsAccount();
    for (size_t i = 0; i < ds.size(); ++i) {
        Account* acc = ds[i];
        if (acc == nullptr) continue;
        
        file << acc->getUsername() << "|";
        file << acc->getPassword() << "|"; // <-- Chinh sua Account.h de them getPassword()
        file << static_cast<int>(acc->getAccountType()) << "|";
        
        if (acc->getLinkedStaff() != nullptr) {
            file << acc->getLinkedStaff()->getID() << "\n";
        } else {
            file << "NULL\n";
        }
    }
    file.close();
    std::cout << "Da luu " << ds.size() << " tai khoan vao " << filePath << std::endl;
}

/**
 * @brief (MOI) Tai file accounts.txt
 */
void FileIO::loadAccounts(AccountManager& am, QuanLy& ql, const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        // std::cerr << "Loi: Khong tim thay file Accounts: " << filePath << std::endl;
        return; // Khong phai la loi lon, co the la chay lan dau
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        MyVector<string> parts = splitString(line, '|');
        if (parts.size() < 4) continue; // Dong bi loi

        string username = parts[0];
        string password = parts[1];
        AccountType type = static_cast<AccountType>(stoi(parts[2]));
        string staffID = parts[3];

        NhanVien* linkedStaff = nullptr;
        if (staffID != "NULL") {
            linkedStaff = ql.getNhanVien(staffID);
            if (linkedStaff == nullptr) {
                // std::cerr << "Loi: Khong tim thay NhanVien " << staffID << " de lien ket tai khoan " << username << std::endl;
                continue; // Bo qua tai khoan nay
            }
        }
        
        am.addAccount(username, password, type, linkedStaff);
        count++;
    }
    file.close();
    std::cout << "Da tai " << count << " tai khoan tu " << filePath << std::endl;
}
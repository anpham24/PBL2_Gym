#pragma once
#include <string>
using namespace std;

class QuanLy;

class FileIO {
public:
    static void saveAllData(const QuanLy& ql, const string& folderPath);
    static void loadAllData(QuanLy& ql, const string& folderPath);
private:
    static void loadHoiVien(QuanLy& ql, const string& filePath);
    static void loadHLV(QuanLy& ql, const string& filePath);
    static void loadNhanVien(QuanLy& ql, const string& filePath);
    static void loadGoiTap(QuanLy& ql, const string& filePath);
    static void loadMonTap(QuanLy& ql, const string& filePath);
    static void loadLopHoc(QuanLy& ql, const string& filePath);
    static void loadHangHoa(QuanLy& ql, const string& filePath);
    static void loadHopDong(QuanLy& ql, const string& filePath);
    static void loadHoaDon(QuanLy& ql, const string& filePath);
    static void saveHoiVien(const QuanLy& ql, const string& filePath);
    static void saveHLV(const QuanLy& ql, const string& filePath);
    static void saveNhanVien(const QuanLy& ql, const string& filePath);
    static void saveGoiTap(const QuanLy& ql, const string& filePath);
    static void saveMonTap(const QuanLy& ql, const string& filePath);
    static void saveLopHoc(const QuanLy& ql, const string& filePath);
    static void saveHangHoa(const QuanLy& ql, const string& filePath);
    static void saveHopDong(const QuanLy& ql, const string& filePath);
    static void saveHoaDon(const QuanLy& ql, const string& filePath);
};
#pragma once
#include <string>
using namespace std;

class QuanLy;
class AccountManager;

class FileIO {
public:
    static void saveAllData(const QuanLy& ql, const AccountManager& am, const string& folderPath);
    static void loadAllData(QuanLy& ql, AccountManager& am, const string& folderPath);
private:
    // Các đối tượng không có khóa ngoại:
    static void loadNhanVien(const string& filePath);
    static void loadHLV(const string& filePath);
    static void loadHoiVien(const string& filePath);
    static void loadMonTap(const string& filePath);
    static void loadGoiTap(const string& filePath);
    static void loadHangHoa(const string& filePath);
    
    // Các đối tượng có khóa ngoại:
    static void loadLopHoc(const string& filePath);
    static void loadHopDong(const string& filePath);
    static void loadHoaDon(const string& filePath);
    static void loadLogTapPT(const string& filePath);

    // Các đối tượng liên kết ngay khi khởi tạo (do không có khóa chính)
    static void loadChiTietHoaDon_HH(const string& filePath);
    static void loadChiTietHoaDon_GT(const string& filePath);

    // Các hàm liên kết khóa ngoại sau khi tất cả dữ liệu đã được tải
    static void linkLopHoc(const string& filePath);
    static void linkHopDong(const string& filePath);
    static void linkHoaDon(const string& filePath);
    static void linkLogTapPT(const string& filePath);
    static void linkGoiTap_MonTap(const string& filePath);

    static void saveNhanVien(const string& filePath);
    static void saveHLV(const string& filePath);
    static void saveHoiVien(const string& filePath);
    static void saveMonTap(const string& filePath);
    static void saveGoiTap(const string& filePath);
    static void saveHangHoa(const string& filePath);

    static void saveLopHoc(const string& filePath);
    static void saveHopDong(const string& filePath);
    static void saveHoaDon(const string& filePath);
    static void saveLogTapPT(const string& filePath);

    static void saveChiTietHoaDon_HH(const string& filePath);
    static void saveChiTietHoaDon_GT(const string& filePath);

    static void saveAccounts(const AccountManager& am, const string& filePath);
    static void loadAccounts(AccountManager& am, QuanLy& ql, const string& filePath);
};
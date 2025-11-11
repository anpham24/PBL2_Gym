#pragma once
#include <string>
#include <ctime>
using namespace std;

class IDGenerator {
private:
    static int nextID[10];
    static int lastYear;
public:
    enum Prefix {
        Prefix_HoiVien = 1,
        Prefix_HLV = 2,
        Prefix_NhanVien = 3,
        Prefix_GoiTap = 4,
        Prefix_MonTap = 5, // Dùng chung prefix với LopHoc
        Prefix_HopDong = 6,
        Prefix_HangHoa = 7,
        Prefix_HoaDon = 8,
        Prefix_LogTapPT = 9
    };
    static string generateID(int prefix);
    static void loadState();
    static void saveState();
};

// ID format: PPYYNNNN (PP: prefix, YY: last two digits of year, NNNN: sequential number)
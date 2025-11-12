#include <string>
using namespace std;

class LopHocService {
public:
    static void themLopHoc(const string& tenLop, const string& lichTap, int thoiLuong,
                           const string& maMon, const string& maHLV);
    static void suaLopHoc(const string& maLop, const string& tenLop, const string& lichTap,
                         int thoiLuong, const string& maMon, const string& maHLV);
    static void xoaLopHoc(const string& maLop);
};
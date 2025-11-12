#pragma once
#include <string>
using namespace std;

class Validator {
public:
    static string validateTen(const string& ten);
    static string validateSDT(const string& sdt);
    static string validateGioiTinh(const string& gioiTinh);
    static string validateNgay(const string& ngay);
    static string validateNgayGio(const string& ngayGio);
    static string validateSoDuong(const int& so);
    static string validateSoDuong(const double& so);
    
};
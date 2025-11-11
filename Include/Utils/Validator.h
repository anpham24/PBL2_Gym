#pragma once
#include <string>
using namespace std;

class Validator {
public:
    static string validateTen(const string& ten);
    static string validateSDT(const string& sdt);
    static string validateGioiTinh(const string& gioiTinh);
    static string validateNgay(const string& ngay);
    static string validateSoDuong(const string& so);
    
};
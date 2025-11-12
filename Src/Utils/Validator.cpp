#include "Validator.h"

string Validator::validateTen(const string& ten) {
    if (ten.empty()) {
        return "Tên không được để trống.";
    }
    if (ten.length() > 50) {
        return "Tên không được vượt quá 50 ký tự.";
    }
    // Kiem tra ký tự hợp lệ (chỉ cho phép chữ cái và dấu cách)
    for (char c : ten) {
        if (!isalpha(c) && !isspace(c)) {
            return "Tên chỉ được chứa chữ cái và dấu cách.";
        }
    }
    return "";
}

string Validator::validateSDT(const string& sdt) {
    if (sdt.length() != 10) {
        return "Số điện thoại phải có đúng 10 chữ số.";
    }
    for (char c : sdt) {
        if (!isdigit(c)) {
            return "Số điện thoại chỉ được chứa chữ số.";
        }
    }
    return "";
}

string Validator::validateGioiTinh(const string& gioiTinh) {
    if (gioiTinh != "Nam" && gioiTinh != "Nữ" && gioiTinh != "Khác") {
        return "Giới tính phải là 'Nam', 'Nữ' hoặc 'Khác'.";
    }
    return "";
}

string Validator::validateNgay(const string& ngay) {
    // Định dạng ngày dự kiến: "DD/MM/YYYY"
    if (ngay.length() != 10 || ngay[2] != '/' || ngay[5] != '/') {
        return "Ngày sinh phải có định dạng DD/MM/YYYY.";
    }
    // Kiểm tra các phần ngày, tháng, năm
    int day = stoi(ngay.substr(0, 2));
    int month = stoi(ngay.substr(3, 2));
    int year = stoi(ngay.substr(6, 4));

    if (month < 1 || month > 12) {
        return "Tháng phải từ 1 đến 12.";
    }
    if (day < 1 || day > 31) {
        return "Ngày phải từ 1 đến 31.";
    }
    // Kiểm tra số ngày hợp lệ theo tháng
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return "Tháng này chỉ có tối đa 30 ngày.";
    }
    if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeapYear ? 29 : 28)) {
            return "Tháng Hai có tối đa " + to_string(isLeapYear ? 29 : 28) + " ngày.";
        }
    }
    return "";
}

string Validator::validateNgayGio(const string& ngayGio) {
    // Định dạng ngày giờ dự kiến: "DD/MM/YYYY HH:MM"
    if (ngayGio.length() != 16 || ngayGio[2] != '/' || ngayGio[5] != '/' || ngayGio[10] != ' ' || ngayGio[13] != ':') {
        return "Ngày giờ phải có định dạng DD/MM/YYYY HH:MM.";
    }
    // Tách phần ngày và giờ
    string ngay = ngayGio.substr(0, 10);
    string gio = ngayGio.substr(11, 5);

    // Kiểm tra phần ngày
    string ngayError = validateNgay(ngay);
    if (!ngayError.empty()) {
        return ngayError;
    }

    // Kiểm tra phần giờ
    int hour = stoi(gio.substr(0, 2));
    int minute = stoi(gio.substr(3, 2));

    if (hour < 0 || hour > 23) {
        return "Giờ phải từ 00 đến 23.";
    }
    if (minute < 0 || minute > 59) {
        return "Phút phải từ 00 đến 59.";
    }

    return "";
}

string Validator::validateSoDuong(const int& so) {
    if (so < 0)
        return "Giá trị phải là số dương";
}

string Validator::validateSoDuong(const double& so) {
    if (so < 0)
        return "Giá trị phải là số dương";
}

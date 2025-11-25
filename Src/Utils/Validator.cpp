#include "Validator.h"

string Validator::validateTen(const string& ten) {
    if (ten.empty()) {
        return "Ten khong duoc de trong.";
    }
    if (ten.length() > 50) {
        return "Ten khong duoc vuot qua 50 ky tu.";
    }
    // Kiem tra ký tự hợp lệ (chỉ cho phép chữ cái và dấu cách)
    // for (char c : ten) {
    //     if (!isalpha(c) && !isspace(c)) {
    //         return "Tên chỉ được chứa chữ cái và dấu cách.";
    //     }
    // }
    return "";
}

string Validator::validateSDT(const string& sdt) {
    if (sdt.length() != 10) {
        return "So dien thoai phai co dung 10 chu so.";
    }
    for (char c : sdt) {
        if (!isdigit(c)) {
            return "So dien thoai chi duoc chua chu so.";
        }
    }
    return "";
}

string Validator::validateGioiTinh(const string& gioiTinh) {
    if (gioiTinh != "Nam" && gioiTinh != "Nu" && gioiTinh != "Khac") {
        return "Gioi tinh phai la 'Nam', 'Nu' hoac 'Khac'.";
    }
    return "";
}

string Validator::validateNgay(const string& ngay) {
    // Định dạng ngày dự kiến: "DD/MM/YYYY"
    if (ngay.length() != 10 || ngay[2] != '/' || ngay[5] != '/') {
        return "Ngay sinh phai co dinh dang DD/MM/YYYY.";
    }
    // Kiểm tra các phần ngày, tháng, năm
    int day = stoi(ngay.substr(0, 2));
    int month = stoi(ngay.substr(3, 2));
    int year = stoi(ngay.substr(6, 4));

    if (month < 1 || month > 12) {
        return "Thang phai tu 1 den 12.";
    }
    if (day < 1 || day > 31) {
        return "Ngay phai tu 1 den 31.";
    }
    // Kiem tra so ngay hop le theo thang
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return "Thang nay chi co toi da 30 ngay.";
    }
    if (month == 2) {
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeapYear ? 29 : 28)) {
            return "Thang Hai co toi da " + to_string(isLeapYear ? 29 : 28) + " ngay.";
        }
    }
    return "";
}

string Validator::validateNgayGio(const string& ngayGio) {
    // Định dạng ngày giờ dự kiến: "DD/MM/YYYY HH:MM"
    if (ngayGio.length() != 16 || ngayGio[2] != '/' || ngayGio[5] != '/' || ngayGio[10] != ' ' || ngayGio[13] != ':') {
        return "Ngay gio phai co dinh dang DD/MM/YYYY HH:MM.";
    }
    // TTach phan ngay va gio
    string ngay = ngayGio.substr(0, 10);
    string gio = ngayGio.substr(11, 5);

    // Kiem tra phan ngay
    string ngayError = validateNgay(ngay);
    if (!ngayError.empty()) {
        return ngayError;
    }

    // Kiểm tra phần giờ
    int hour = stoi(gio.substr(0, 2));
    int minute = stoi(gio.substr(3, 2));

    if (hour < 0 || hour > 23) {
        return "Gio phai tu 00 den 23.";
    }
    if (minute < 0 || minute > 59) {
        return "Phut phai tu 00 den 59.";
    }

    return "";
}

// --- SỬA LỖI Ở ĐÂY ---
string Validator::validateSoDuong(const int& so) {
    if (so < 0)
        return "Gia tri phai la so duong";
    return ""; // <--- THÊM DÒNG NÀY: Trả về rỗng nếu không lỗi
}

string Validator::validateSoDuong(const double& so) {
    if (so < 0)
        return "Gia tri phai la so duong";
    return ""; // <--- THÊM DÒNG NÀY
}
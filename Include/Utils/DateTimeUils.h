// Utils/DateTimeUtils.h
#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

class DateTimeUtils {
public:
    // Tra ve chuoi co dang "DD/MM/YYYY HH:MM"
    static std::string getCurrentDateTime() {
        auto t = std::time(nullptr);
        // Su dung localtime_s de an toan hon tren Windows,
        // nhung localtime() pho bien hon
        auto tm = *std::localtime(&t);
        
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d/%m/%Y %H:%M");
        return oss.str();
    }

    static std::string addMonths(const std::string& date, int months) {
     std::cout << "📅 addMonths: " << date << " + " << months << " months" << std::endl;
    
    // Format: DD/MM/YYYY
    std::istringstream ss(date);
    int day, month, year;
    char delimiter;
    
    ss >> day >> delimiter >> month >> delimiter >> year;
    
    std::cout << "   Parsed: " << day << "/" << month << "/" << year << std::endl;
    
    // ✅ FIX: Cộng tháng ĐÚNG
    month += months;
    
    // Xử lý tràn năm
    while (month > 12) {
        month -= 12;
        year++;
    }
    
    // ✅ Xử lý ngày không hợp lệ (VD: 31/02 → 28/02)
    int maxDay = 31;
    if (month == 2) {
        // Năm nhuận
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            maxDay = 29;
        } else {
            maxDay = 28;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    }
    
    if (day > maxDay) {
        day = maxDay;
    }
    
    // Format lại
    std::ostringstream result;
    result << std::setfill('0') << std::setw(2) << day << "/"
           << std::setfill('0') << std::setw(2) << month << "/"
           << year;
    
    std::cout << "   Result: " << result.str() << std::endl;
    
    return result.str();
}
    static std::string addDays(const std::string& date, int days) {
    // Format: DD/MM/YYYY
    std::istringstream ss(date);
    int day, month, year;
    char delimiter;
    
    ss >> day >> delimiter >> month >> delimiter >> year;
    
    // Convert to time_t
    tm t = {};
    t.tm_mday = day;
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;
    
    time_t timeValue = mktime(&t);
    
    // Cộng số ngày (1 ngày = 86400 giây)
    timeValue += days * 86400;
    
    // Convert back to tm
    tm* newTime = localtime(&timeValue);
    
    // Format lại
    std::ostringstream result;
    result << std::setfill('0') << std::setw(2) << newTime->tm_mday << "/"
           << std::setfill('0') << std::setw(2) << (newTime->tm_mon + 1) << "/"
           << (newTime->tm_year + 1900);
    
    return result.str();
}

static long dateToInt(const string& date) {
    if (date.length() < 10) return 0;
    
    // Parse DD/MM/YYYY
    int day = std::stoi(date.substr(0, 2));
    int month = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));
    
    // Công thức: year * 10000 + month * 100 + day
    // VD: 15/11/2025 → 20251115
    return year * 10000 + month * 100 + day;
}

static bool isBetween(const string& date, const string& startDate, const string& endDate) {
    long dateInt = dateToInt(date);
    long startInt = dateToInt(startDate);
    long endInt = dateToInt(endDate);
    
    return (dateInt >= startInt && dateInt <= endInt);
}
};
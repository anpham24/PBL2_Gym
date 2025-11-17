// Utils/DateTimeUtils.h
#pragma once
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

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
};
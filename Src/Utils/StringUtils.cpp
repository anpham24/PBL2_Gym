#include "StringUtils.h"
#include <cctype>

MyVector<string> splitString(const string& str, char delimiter) {
    MyVector<string> parts;
    if (str.empty()) {
        return parts;
    }

    size_t startPos = 0;
    size_t endPos = str.find(delimiter, startPos);

    // Lặp qua chuỗi để tìm các phần tử
    while (endPos != string::npos) {
        // Trích xuất phần tử: từ startPos với độ dài endPos - startPos
        parts.push_back(str.substr(startPos, endPos - startPos));
        
        // Di chuyển đến vị trí tiếp theo (sau dấu phân cách)
        startPos = endPos + 1;
        
        // Tìm dấu phân cách tiếp theo
        endPos = str.find(delimiter, startPos);
    }

    // Thêm phần còn lại của chuỗi (sau dấu phân cách cuối cùng hoặc toàn bộ chuỗi nếu không có delimiter)
    parts.push_back(str.substr(startPos));

    return parts;
}

bool toBool(const string& str) {
    return str == "1" || str == "true" || str == "True" || str == "TRUE";
}

std::string StringUtils::toLower(const std::string& str) {
    std::string result = str;
    
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    
    return result;
}

// ✅ Tự implement: Tìm substring (Naive approach - yêu cầu giảng viên)
int StringUtils::findSubstring(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return 0;
    if (text.length() < pattern.length()) return -1;
    
    size_t textLen = text.length();
    size_t patternLen = pattern.length();
    
    // Duyệt từng vị trí trong text
    for (size_t i = 0; i <= textLen - patternLen; ++i) {
        bool found = true;
        
        // So sánh từng ký tự
        for (size_t j = 0; j < patternLen; ++j) {
            if (text[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        
        if (found) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

// ✅ Kiểm tra chứa substring (case-insensitive)
bool StringUtils::contains(const std::string& text, const std::string& pattern) {
    std::string lowerText = toLower(text);
    std::string lowerPattern = toLower(pattern);
    
    return findSubstring(lowerText, lowerPattern) != -1;
}

// ✅ So sánh 2 chuỗi (case-insensitive)
bool StringUtils::equals(const std::string& str1, const std::string& str2) {
    if (str1.length() != str2.length()) return false;
    
    std::string lower1 = toLower(str1);
    std::string lower2 = toLower(str2);
    
    return lower1 == lower2; // ✅ Dùng == có sẵn
}

// ✅ Kiểm tra chuỗi có phải số không (dùng std::isdigit)
bool StringUtils::isNumber(const std::string& str) {
    if (str.empty()) return false;
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            return false;
        }
    }
    
    return true;
}
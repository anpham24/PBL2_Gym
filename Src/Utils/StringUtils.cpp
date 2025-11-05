#include "StringUtils.h"

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
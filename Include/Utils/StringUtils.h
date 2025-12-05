#pragma once
#include <string>
#include "MyVector.h"

using namespace std;

MyVector<string> splitString(const string& str, char delimiter);
bool toBool(const string& str);

class StringUtils {
public:
    static std::string toLower(const std::string& str);
    static int findSubstring(const std::string& text, const std::string& pattern);
    static bool contains(const std::string& text, const std::string& pattern);
    static bool equals(const std::string& str1, const std::string& str2);
    static bool isNumber(const std::string& str);
};
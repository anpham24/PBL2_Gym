#include "../../Include/UI/UIComponents.h"
#include <ctime>
#include <algorithm>
#include <cctype>
#include <regex>

namespace UIComponents {

// ============================================================================
// STRING FORMATTING
// ============================================================================

string formatMoney(double amount) {
    stringstream ss;
    ss << fixed << setprecision(0) << amount;
    string result = ss.str();
    
    int pos = result.length() - 3;
    while (pos > 0) {
        result.insert(pos, ",");
        pos -= 3;
    }
    return result + " VND";
}

string formatDate(const string& date) {
    // Assume input format: DD/MM/YYYY
    // Output: DD Thang MM, YYYY
    if (date.length() < 10) return date;
    
    string day = date.substr(0, 2);
    string month = date.substr(3, 2);
    string year = date.substr(6, 4);
    
    return day + " Thang " + month + ", " + year;
}

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
       << setw(2) << (1 + ltm->tm_mon) << "/"
       << (1900 + ltm->tm_year);
    return ss.str();
}

string calculateDateAfterMonths(int months) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    ltm->tm_mon += months;
    mktime(ltm);
    
    stringstream ss;
    ss << setfill('0') << setw(2) << ltm->tm_mday << "/"
       << setw(2) << (1 + ltm->tm_mon) << "/"
       << (1900 + ltm->tm_year);
    return ss.str();
}

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string truncateString(const string& str, size_t maxLength) {
    if (str.length() <= maxLength) return str;
    return str.substr(0, maxLength - 3) + "...";
}

// ============================================================================
// VALIDATION
// ============================================================================

bool isValidEmail(const string& email) {
    // Simple email validation
    const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return regex_match(email, pattern);
}

bool isValidPhone(const string& phone) {
    // Vietnamese phone: 10 digits, starts with 0
    if (phone.length() != 10) return false;
    if (phone[0] != '0') return false;
    
    for (char c : phone) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isNumeric(const string& str) {
    if (str.empty()) return false;
    
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') start = 1;
    
    bool hasDecimal = false;
    for (size_t i = start; i < str.length(); i++) {
        if (str[i] == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool isEmpty(const string& str) {
    return str.empty() || 
           all_of(str.begin(), str.end(), [](char c) { return isspace(c); });
}

// ============================================================================
// DRAWING HELPERS
// ============================================================================

void drawStatCard(RenderWindow& window, Font& font,
                 const string& title, const string& value,
                 float x, float y, float width, float height,
                 Color color) {
    RectangleShape card({width, height});
    card.setPosition({x, y});
    card.setFillColor(color);
    window.draw(card);
    
    Text titleText(font, title, 14);
    titleText.setFillColor(Color(255, 255, 255, 180));
    titleText.setPosition({x + 15, y + 15});
    window.draw(titleText);
    
    Text valueText(font, value, 32);
    valueText.setFillColor(Color::White);
    valueText.setStyle(Text::Bold);
    valueText.setPosition({x + 15, y + 45});
    window.draw(valueText);
}

void drawSeparator(RenderWindow& window, float x, float y, 
                  float width, Color color) {
    RectangleShape line({width, 2});
    line.setPosition({x, y});
    line.setFillColor(color);
    window.draw(line);
}

void drawTableHeader(RenderWindow& window, Font& font,
                    const vector<string>& headers,
                    const vector<float>& positions,
                    float y, float width, float height) {
    RectangleShape headerBox({width, height});
    headerBox.setPosition({positions[0] - 20, y});
    headerBox.setFillColor(Colors::Primary);
    window.draw(headerBox);
    
    for (size_t i = 0; i < headers.size() && i < positions.size(); i++) {
        Text header(font, headers[i], 16);
        header.setFillColor(Color::White);
        header.setStyle(Text::Bold);
        header.setPosition({positions[i], y + 10});
        window.draw(header);
    }
}

void drawTableRow(RenderWindow& window, Font& font,
                 const vector<string>& cells,
                 const vector<float>& positions,
                 float y, float width, float height,
                 bool isSelected, bool isEven) {
    RectangleShape row({width, height});
    row.setPosition({positions[0] - 20, y});
    
    if (isSelected) {
        row.setFillColor(Color(200, 220, 240));
    } else {
        row.setFillColor(isEven ? Color(250, 250, 250) : Color::White);
    }
    
    row.setOutlineColor(Color(230, 230, 230));
    row.setOutlineThickness(1);
    window.draw(row);
    
    for (size_t i = 0; i < cells.size() && i < positions.size(); i++) {
        Text cell(font, cells[i], 14);
        cell.setFillColor(Colors::TextPrimary);
        cell.setPosition({positions[i], y + 15});
        window.draw(cell);
    }
}

void drawMessageBox(RenderWindow& window, Font& font,
                   const string& message, const string& title,
                   float x, float y, float width, float height,
                   Color color) {
    // Background overlay
    RectangleShape overlay({1400, 900});
    overlay.setFillColor(Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Message box
    RectangleShape box({width, height});
    box.setPosition({x, y});
    box.setFillColor(Color::White);
    box.setOutlineColor(color);
    box.setOutlineThickness(3);
    window.draw(box);
    
    // Title
    Text titleText(font, title, 24);
    titleText.setFillColor(color);
    titleText.setStyle(Text::Bold);
    titleText.setPosition({x + 20, y + 20});
    window.draw(titleText);
    
    // Message
    Text messageText(font, message, 18);
    messageText.setFillColor(Colors::TextPrimary);
    messageText.setPosition({x + 20, y + 70});
    window.draw(messageText);
}

void centerText(Text& text, FloatRect bounds) {
    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({textBounds.size.x / 2.f, textBounds.size.y / 2.f});
    text.setPosition({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
}

RectangleShape createShadow(const RectangleShape& shape, 
                           float offsetX, float offsetY) {
    RectangleShape shadow = shape;
    Vector2f pos = shape.getPosition();
    shadow.setPosition({pos.x + offsetX, pos.y + offsetY});
    shadow.setFillColor(Color(0, 0, 0, 50));
    shadow.setOutlineThickness(0);
    return shadow;
}

// ============================================================================
// PAGINATION HELPERS
// ============================================================================

int calculateTotalPages(int totalItems, int itemsPerPage) {
    if (itemsPerPage <= 0) return 1;
    return (totalItems + itemsPerPage - 1) / itemsPerPage;
}

int getPageStartIndex(int currentPage, int itemsPerPage) {
    return currentPage * itemsPerPage;
}

int getPageEndIndex(int currentPage, int itemsPerPage, int totalItems) {
    int startIdx = getPageStartIndex(currentPage, itemsPerPage);
    return min(startIdx + itemsPerPage, totalItems);
}

void drawPagination(RenderWindow& window, Font& font,
                   int currentPage, int totalPages,
                   float x, float y) {
    // Current page info
    stringstream ss;
    ss << "Trang " << (currentPage + 1) << " / " << totalPages;
    
    Text pageInfo(font, ss.str(), 16);
    pageInfo.setFillColor(Colors::TextSecondary);
    FloatRect bounds = pageInfo.getLocalBounds();
    pageInfo.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    pageInfo.setPosition({x, y});
    window.draw(pageInfo);
}

} // namespace UIComponents
#ifndef UICOMPONENTS_H
#define UICOMPONENTS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <iomanip>

using namespace sf;
using namespace std;

namespace UIComponents {
    // ============================================================================
    // COLORS
    // ============================================================================
    namespace Colors {
        const Color Primary(70, 130, 180);        // Steel Blue
        const Color PrimaryHover(100, 160, 210);
        const Color PrimaryClick(50, 110, 160);
        
        const Color Success(80, 180, 100);        // Green
        const Color Warning(220, 180, 80);        // Yellow
        const Color Danger(220, 100, 80);         // Red
        const Color Info(70, 130, 180);           // Blue
        
        const Color Background(245, 247, 250);    // Light Gray
        const Color Surface(255, 255, 255);       // White
        const Color Border(200, 200, 200);        // Gray
        
        const Color TextPrimary(50, 50, 50);      // Dark Gray
        const Color TextSecondary(100, 100, 100); // Medium Gray
        const Color TextDisabled(150, 150, 150);  // Light Gray
    }

    // ============================================================================
    // HELPER FUNCTIONS
    // ============================================================================
    
    /**
     * Format money with thousands separator
     * Example: 1000000 -> "1,000,000 VND"
     */
    string formatMoney(double amount);
    
    /**
     * Format date from DD/MM/YYYY to readable format
     */
    string formatDate(const string& date);
    
    /**
     * Get current date as string (DD/MM/YYYY)
     */
    string getCurrentDate();
    
    /**
     * Calculate date after X months from current date
     */
    string calculateDateAfterMonths(int months);
    
    /**
     * Convert string to lowercase
     */
    string toLower(const string& str);
    
    /**
     * Truncate string if too long and add "..."
     */
    string truncateString(const string& str, size_t maxLength);
    
    /**
     * Validate email format
     */
    bool isValidEmail(const string& email);
    
    /**
     * Validate phone number (Vietnamese format)
     */
    bool isValidPhone(const string& phone);
    
    /**
     * Check if string is numeric
     */
    bool isNumeric(const string& str);
    
    /**
     * Check if string is empty or whitespace only
     */
    bool isEmpty(const string& str);

    // ============================================================================
    // DRAWING HELPERS
    // ============================================================================
    
    /**
     * Draw a stat card (colored rectangle with title and value)
     */
    void drawStatCard(RenderWindow& window, Font& font,
                     const string& title, const string& value,
                     float x, float y, float width, float height,
                     Color color);
    
    /**
     * Draw a separator line
     */
    void drawSeparator(RenderWindow& window, float x, float y, 
                      float width, Color color = Colors::Border);
    
    /**
     * Draw a table header row
     */
    void drawTableHeader(RenderWindow& window, Font& font,
                        const vector<string>& headers,
                        const vector<float>& positions,
                        float y, float width, float height);
    
    /**
     * Draw a table row
     */
    void drawTableRow(RenderWindow& window, Font& font,
                     const vector<string>& cells,
                     const vector<float>& positions,
                     float y, float width, float height,
                     bool isSelected = false, bool isEven = true);
    
    /**
     * Draw a message box
     */
    void drawMessageBox(RenderWindow& window, Font& font,
                       const string& message, const string& title,
                       float x, float y, float width, float height,
                       Color color = Colors::Info);
    
    /**
     * Draw a confirmation dialog
     */
    bool drawConfirmDialog(RenderWindow& window, Font& font,
                          const string& message, const string& title,
                          float x, float y, float width, float height);
    
    /**
     * Center text in a rectangle
     */
    void centerText(Text& text, FloatRect bounds);
    
    /**
     * Create shadow effect for rectangle
     */
    RectangleShape createShadow(const RectangleShape& shape, 
                               float offsetX = 2, float offsetY = 2);

    // ============================================================================
    // PAGINATION HELPERS
    // ============================================================================
    
    /**
     * Calculate total pages
     */
    int calculateTotalPages(int totalItems, int itemsPerPage);
    
    /**
     * Get start index for current page
     */
    int getPageStartIndex(int currentPage, int itemsPerPage);
    
    /**
     * Get end index for current page
     */
    int getPageEndIndex(int currentPage, int itemsPerPage, int totalItems);
    
    /**
     * Draw pagination controls
     */
    void drawPagination(RenderWindow& window, Font& font,
                       int currentPage, int totalPages,
                       float x, float y);
}

#endif // UICOMPONENTS_H
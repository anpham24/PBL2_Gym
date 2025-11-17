// GUI/Components/Table.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include "Config.h"
#include "Utils/MyVector.h" // De nhan du lieu

// ========================================================================
// DINH NGHIA COT (STRUCTS)
// ========================================================================

/**
 * @brief Dinh nghia mot cot du lieu (VD: "Ten", "ID").
 */
template <typename T>
struct ColumnDefinition {
    std::string headerName; // Ten hien thi o dau cot
    float width;            // Chieu rong cot
    
    // Ham de trich xuat chuoi string tu doi tuong data
    std::function<std::string(const T*)> dataExtractor; 
};

/**
 * @brief Dinh nghia mot cot hanh dong (VD: "Xem", "Sua", "Xoa").
 */
template <typename T>
struct ActionColumn {
    std::string actionName; // Chu hien thi (VD: "Sua")
    float width;            // Chieu rong cot
    
    // Ham se duoc goi khi click vao hanh dong nay
    std::function<void(T*)> onActionClick; 
    
    sf::Color color; // Mau sac cua chu (VD: Mau do cho "Xoa")
};


// ========================================================================
// LOP TABLE (HEADER)
// ========================================================================

template <typename T>
class Table {
private:
    sf::Font& font;
    sf::RectangleShape headerBg; // Nen cua hang tieu de
    sf::RectangleShape rowBg;       // Mau hang chan
    sf::RectangleShape rowBgAlt;    // Mau hang le (de xen ke)
    sf::RectangleShape hoverBg;     // Mau khi di chuot qua

    sf::Text headerText; // Text dung cho tieu de
    sf::Text cellText;   // Text dung cho cac o du lieu

    // Danh sach cac cot
    std::vector<ColumnDefinition<T>> dataColumns;
    std::vector<ActionColumn<T>> actionColumns;
    
    // Du lieu hien thi (da duoc phan trang tu Screen)
    std::vector<T*> displayedItems; 

    sf::Vector2f position;
    float rowHeight;
    float headerHeight;
    float tableWidth; // Tong chieu rong cua bang

    int hoveredRow; // Index cua hang dang duoc di chuot qua

    /**
     * @brief Ham noi bo de ve mot o (cell) voi padding
     */
    void drawCell(sf::RenderTarget& target, const std::string& text, sf::Vector2f pos, float width, sf::Color color, bool isHeader = false);

public:
    /**
     * @brief Ham dung, yeu cau mot font chu.
     * @param font Font toan cuc tu App.
     * @param rowHeight Chieu cao cua moi hang du lieu.
     */
    Table(sf::Font& font, float rowHeight = 40.0f);

    ~Table();

    // --- Thiet lap cot ---
    
    /**
     * @brief Them mot cot du lieu vao bang.
     * @param header Ten hien thi o tieu de.
     * @param width Chieu rong cot (pixels).
     * @param extractor Mot ham lambda (hoac function) de lay chuoi tu data.
     */
    void addColumn(const std::string& header, float width, std::function<std::string(const T*)> extractor);
    
    /**
     * @brief Them mot cot hanh dong tuy chinh.
     */
    void addAction(const std::string& actionName, std::function<void(T*)> onClick, sf::Color color, float width = 80.0f);
    
    // --- Cac ham tien ich (Helpers) ---
    void addViewAction(std::function<void(T*)> onClick);
    void addEditAction(std::function<void(T*)> onClick);
    void addDeleteAction(std::function<void(T*)> onClick);

    /**
     * @brief Xoa tat ca cac cot da dinh nghia (de tai su dung bang cho tab khac).
     */
    void clearColumns();
    
    /**
     * @brief Dat vi tri cho goc tren ben trai cua bang.
     */
    void setPosition(float x, float y);
    
    /**
     * @brief Nap du lieu (da phan trang) vao bang.
     * @param data Mot MyVector chua cac con tro den data cua trang hien tai.
     */
    void setData(const MyVector<T*>& data);

    // --- Vong lap chinh ---
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void draw(sf::RenderTarget& target);
};


// ========================================================================
// LOP TABLE (IMPLEMENTATION - TRUOC DAY LA .TPP)
// ========================================================================

// --- Constructor & Destructor ---
template <typename T>
Table<T>::Table(sf::Font& font, float rowHeight) 
    : font(font), 
      rowHeight(rowHeight), 
      headerHeight(45.0f), 
      tableWidth(0), 
      hoveredRow(-1), 
      position(0, 0),
      headerText(font, "", 16), 
      cellText(font, "", 16)
{
    headerBg.setFillColor(Config::CardDark);
    rowBg.setFillColor(Config::CardLight);
    rowBgAlt.setFillColor(sf::Color(49, 56, 66)); // Toi hon CardDark mot chut
    hoverBg.setFillColor(sf::Color(65, 75, 88)); // Mau khi hover
    
    // headerText.setFont(font);
    // headerText.setCharacterSize(16);
    headerText.setFillColor(Config::TextMuted);
    headerText.setStyle(sf::Text::Bold);

    // cellText.setFont(font);
    // cellText.setCharacterSize(16);
    cellText.setFillColor(Config::TextNormal);
}

template <typename T>
Table<T>::~Table() {
    // Table khong so huu con tro T*, nen khong can delete
}

// --- Setup ---
template <typename T>
void Table<T>::addColumn(const std::string& header, float width, std::function<std::string(const T*)> extractor) {
    dataColumns.push_back({header, width, extractor});
    tableWidth += width;
}

template <typename T>
void Table<T>::addAction(const std::string& actionName, std::function<void(T*)> onClick, sf::Color color, float width) {
    actionColumns.push_back({actionName, width, onClick, color});
    tableWidth += width;
}

template <typename T>
void Table<T>::addViewAction(std::function<void(T*)> onClick) {
    addAction("Xem", onClick, Config::AccentCyan, 80.0f);
}

template <typename T>
void Table<T>::addEditAction(std::function<void(T*)> onClick) {
    addAction("Sua", onClick, Config::Warning, 80.0f);
}

template <typename T>
void Table<T>::addDeleteAction(std::function<void(T*)> onClick) {
    addAction("Xoa", onClick, Config::Danger, 80.0f);
}

template <typename T>
void Table<T>::clearColumns() {
    dataColumns.clear();
    actionColumns.clear();
    tableWidth = 0;
}

template <typename T>
void Table<T>::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
}

template <typename T>
void Table<T>::setData(const MyVector<T*>& data) {
    displayedItems.clear();
    // Sao chep con tro tu MyVector (data da phan trang) sang std::vector noi bo
    for (size_t i = 0; i < data.size(); ++i) {
        displayedItems.push_back(data[i]);
    }
    hoveredRow = -1; // Reset hover
}

// --- Event & Update (SFML 3.0) ---
template <typename T>
void Table<T>::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    // Kiem tra su kien nhan chuot trai
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button != sf::Mouse::Button::Left) {
            return;
        }
    } else {
        return; // Khong phai su kien nhan chuot
    }
    
    // Neu click khong nam trong hang nao
    if (hoveredRow < 0 || hoveredRow >= displayedItems.size()) {
        return;
    }

    // --- Kiem tra xem co click vao cot hanh dong khong ---
    float currentX = position.x;
    T* clickedItem = displayedItems[hoveredRow];
    
    // 1. Bo qua tat ca cac cot du lieu
    for (const auto& col : dataColumns) {
        currentX += col.width;
    }
    
    // 2. Kiem tra trong cac cot hanh dong
    for (const auto& actionCol : actionColumns) {
        if (mousePos.x >= currentX && mousePos.x < (currentX + actionCol.width)) {
            // Da click trung vao cot hanh dong nay!
            if (actionCol.onActionClick) {
                actionCol.onActionClick(clickedItem); // Goi callback
            }
            return; // Chi xu ly mot hanh dong moi lan click
        }
        currentX += actionCol.width;
    }
}

template <typename T>
void Table<T>::update(sf::Vector2i mousePos) {
    // Tinh toan hang (row) dang duoc di chuot qua
    
    // Kiem tra neu chuot nam ngoai chieu rong bang
    if (mousePos.x < position.x || mousePos.x > (position.x + tableWidth)) {
        hoveredRow = -1;
        return;
    }
    
    float yStart = position.y + headerHeight; // Vi tri Y cua hang 0
    
    // Kiem tra neu chuot nam tren hang tieu de
    if (mousePos.y < yStart) {
        hoveredRow = -1;
        return;
    }

    // Tinh index cua hang
    int row = static_cast<int>((mousePos.y - yStart) / rowHeight);
    
    if (row >= 0 && row < displayedItems.size()) {
        hoveredRow = row; // Dat hang duoc hover
    } else {
        hoveredRow = -1; // Nam ben duoi hang cuoi cung
    }
}

// --- Draw (SFML 3.0) ---
template <typename T>
void Table<T>::draw(sf::RenderTarget& target) {
    float currentX = position.x;
    float currentY = position.y;

    // 1. Ve Header
    headerBg.setSize(sf::Vector2f(tableWidth, headerHeight));
    headerBg.setPosition(position);
    target.draw(headerBg);
    
    // Ve ten cac cot data
    for (const auto& col : dataColumns) {
        drawCell(target, col.headerName, sf::Vector2f(currentX, currentY), col.width, Config::TextMuted, true);
        currentX += col.width;
    }
    // Ve ten cac cot action
    for (const auto& col : actionColumns) {
        drawCell(target, col.actionName, sf::Vector2f(currentX, currentY), col.width, Config::TextMuted, true);
        currentX += col.width;
    }

    // 2. Ve cac hang du lieu
    currentY += headerHeight;
    for (size_t i = 0; i < displayedItems.size(); ++i) {
        currentX = position.x;
        T* item = displayedItems[i];
        
        // Chon mau hang (chan/le xen ke)
        sf::RectangleShape& bg = (i % 2 == 0) ? rowBg : rowBgAlt;
        bg.setSize(sf::Vector2f(tableWidth, rowHeight));
        bg.setPosition(sf::Vector2f(currentX, currentY));
        target.draw(bg);
        
        // Neu hang nay dang duoc hover, ve lop highlight de len
        if (i == hoveredRow) {
            hoverBg.setSize(sf::Vector2f(tableWidth, rowHeight));
            hoverBg.setPosition(sf::Vector2f(currentX, currentY));
            target.draw(hoverBg);
        }
        
        // Ve du lieu cac cot
        for (const auto& col : dataColumns) {
            std::string data = col.dataExtractor(item);
            drawCell(target, data, sf::Vector2f(currentX, currentY), col.width, Config::TextNormal, false);
            currentX += col.width;
        }
        
        // Ve cac cot hanh dong (voi mau sac tuy chinh)
        for (const auto& col : actionColumns) {
            drawCell(target, col.actionName, sf::Vector2f(currentX, currentY), col.width, col.color, false);
            currentX += col.width;
        }

        currentY += rowHeight;
    }
}

/**
 * @brief Ham noi bo de ve mot o (cell) voi padding
 */
template <typename T>
void Table<T>::drawCell(sf::RenderTarget& target, const std::string& text, sf::Vector2f pos, float width, sf::Color color, bool isHeader) {
    
    sf::Text& textObj = isHeader ? headerText : cellText;
    float height = isHeader ? headerHeight : rowHeight;

    textObj.setString(text);
    textObj.setFillColor(color);
    
    // Canh giua cell theo chieu doc
    float yPos = pos.y + (height - textObj.getCharacterSize()) / 2.0f - 2.0f;
    // Canh trai cell theo chieu ngang (co padding 10px)
    float xPos = pos.x + 10.0f; 

    textObj.setPosition(sf::Vector2f(xPos, yPos));
    target.draw(textObj);
}
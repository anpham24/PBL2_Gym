// GUI/Components/TabManager.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Config.h" // De lay bang mau

/*
 * Lop TabManager quan ly mot nhom cac nhan (label) co the click de chuyen tab.
 * No chi quan ly trang thai tab nao dang active, khong quan ly noi dung.
 */
class TabManager {
private:
    sf::Font& font;
    sf::Vector2f position;
    
    // Su dung std::vector vi chung ta khong luu con tro
    std::vector<sf::Text> tabLabels; // Danh sach cac nhan (VD: "Thong tin", "Goi tap")
    sf::RectangleShape activeLine; // Duong gach chan tab dang chon
    
    int activeTabIndex; // Index cua tab dang duoc chon

public:
    /**
     * @brief Ham dung.
     * @param font Font toan cuc tu App.
     */
    TabManager(sf::Font& font);
    ~TabManager();
    
    /**
     * @brief Dat vi tri cho thanh tab.
     */
    void setPosition(float x, float y);
    
    /**
     * @brief Them mot tab moi vao cuoi danh sach.
     * @param label Ten hien thi cua tab.
     */
    void addTab(const std::string& label);
    
    /**
     * @brief Lay index cua tab dang active (0-based).
     */
    int getActiveTab() const;

    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void draw(sf::RenderTarget& target);
};
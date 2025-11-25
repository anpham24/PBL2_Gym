// GUI/Popups/HLVDetailPopup.h
#pragma once

#include "BasePopup.h"
#include "TabManager.h"
#include "HLV.h"
#include <vector>
#include <string>

/*
 * Lop HLVDetailPopup hien thi chi tiet thong tin cua mot HLV.
 * No su dung TabManager de chia thanh 2 tab:
 * 1. Thong tin ca nhan.
 * 2. Chi tiet tinh luong.
 */
class HLVDetailPopup : public BasePopup {
private:
    HLV* currentHLV; // HLV dang duoc xem
    TabManager tabManager;

    // --- Tab 1: Thong Tin Ca Nhan ---
    std::vector<sf::Text> infoLines; // Danh sach cac dong text (Label: Value)
    
    // --- Tab 2: Chi Tiet Luong ---
    std::vector<sf::Text> luongLines; // Danh sach cac dong text tinh luong

    /**
     * @brief Tai du lieu tu currentHLV vao cac vector (infoLines, luongLines).
     */
    void loadData();
    
    /**
     * @brief Ve noi dung cho Tab 1.
     */
    void drawTabInfo(sf::RenderTarget& target);
    
    /**
     * @brief Ve noi dung cho Tab 2.
     */
    void drawTabLuong(sf::RenderTarget& target);
    
    /**
     * @brief Ham Helper: Tao va them 2 sf::Text (label, value) vao mot vector.
     */
    void createInfoLine(std::vector<sf::Text>& lines, const std::string& label, const std::string& value, float yPos);

protected:
    /**
     * @brief Ghi de ham ao: Ve noi dung dua tren tab dang active.
     */
    void drawContent(sf::RenderTarget& target) override;

public:
    HLVDetailPopup(App& app);

    bool isStaffReadOnly;
    
    /**
     * @brief Hien thi popup voi thong tin cua mot HLV cu the.
     * @param hlv Con tro den HLV can xem.
     */
    void show(HLV* hlv);
    
    /**
     * @brief An popup va xoa du lieu tam.
     */
    void hide() override;

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    
    // Khong can ham update() vi day la popup chi doc (read-only)
    void update(sf::Vector2i mousePos) override; 
};
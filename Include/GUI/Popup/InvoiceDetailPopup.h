// GUI/Popups/InvoiceDetailPopup.h
#pragma once

#include "BasePopup.h"
#include "HoaDon.h"
#include <vector>
#include <string>

/*
 * Lop InvoiceDetailPopup hien thi chi tiet mot hoa don DA THANH TOAN.
 * Day la popup chi doc (read-only).
 */
class InvoiceDetailPopup : public BasePopup {
private:
    HoaDon* currentHoaDon; // Hoa don dang duoc xem
    
    // Su dung vector de luu cac dong text da duoc dinh dang
    std::vector<sf::Text> infoLines;  // Cot 1: Thong tin
    std::vector<sf::Text> itemLines;  // Cot 2: Chi tiet item
    sf::Text totalLine; // Dong tong tien cuoi cung

    /**
     * @brief Tai du lieu tu currentHoaDon vao cac vector (infoLines, itemLines).
     */
    void loadData();

protected:
    /**
     * @brief Ghi de ham ao: Ve noi dung thong tin va chi tiet.
     */
    void drawContent(sf::RenderTarget& target) override;

public:
    InvoiceDetailPopup(App& app);
    
    /**
     * @brief Hien thi popup voi thong tin cua mot Hoa Don cu the.
     * @param hd Con tro den HoaDon can xem.
     */
    void show(HoaDon* hd);
    
    /**
     * @brief An popup va xoa du lieu tam.
     */
    void hide() override;
    
    // Khong can handleEvent() vi popup nay chi doc
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    
    // Khong can update() vi popup nay chi doc
    void update(sf::Vector2i mousePos) override; 
};
// GUI/Popups/NhanVienDetailPopup.h
#pragma once

#include "BasePopup.h"
#include "NhanVien.h"
#include <vector>
#include <string>

/*
 * Lop NhanVienDetailPopup hien thi chi tiet thong tin cua mot Nhan Vien.
 * No khong co tab, hien thi thong tin ca nhan va chi tiet luong
 * tren cung mot panel.
 */
class NhanVienDetailPopup : public BasePopup {
private:
    NhanVien* currentNhanVien; // Nhan Vien dang duoc xem
    
    // Su dung vector de luu cac dong text da duoc dinh dang
    std::vector<sf::Text> infoLines;  // Thong tin ca nhan
    std::vector<sf::Text> luongLines; // Chi tiet luong

    /**
     * @brief Tai du lieu tu currentNhanVien vao cac vector (infoLines, luongLines).
     */
    void loadData();
    
    /**
     * @brief Ham Helper: Tao va them 2 sf::Text (label, value) vao mot vector.
     * @param lines Vector de them vao (infoLines hoac luongLines).
     * @param label Nhan ben trai (VD: "Ho Ten").
     * @param value Gia tri ben phai (VD: "Nguyen Van A").
     * @param yPos Vi tri Y cua dong.
     */
    void createInfoLine(std::vector<sf::Text>& lines, const std::string& label, const std::string& value, float yPos);

protected:
    /**
     * @brief Ghi de ham ao: Ve noi dung thong tin va luong.
     */
    void drawContent(sf::RenderTarget& target) override;

public:
    NhanVienDetailPopup(App& app);
    
    /**
     * @brief Hien thi popup voi thong tin cua mot Nhan Vien cu the.
     * @param nv Con tro den NhanVien can xem.
     */
    void show(NhanVien* nv);

    bool isStaffReadOnly;
    
    /**
     * @brief An popup va xoa du lieu tam.
     */
    void hide() override;
    
    // Khong can handleEvent() vi popup nay chi doc
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    
    // Khong can update() vi popup nay chi doc
    void update(sf::Vector2i mousePos) override; 
};
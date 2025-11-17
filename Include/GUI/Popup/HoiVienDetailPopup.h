// GUI/Popups/HoiVienDetailPopup.h
#pragma once

#include "BasePopup.h"
#include "TabManager.h"
#include "Button.h"
#include "HoiVien.h"
#include "HopDong.h"
#include "GoiTap.h"
#include <vector>
#include <string>

/*
 * (Component con)
 * Dinh nghia mot hang (row) trong danh sach goi tap (o Tab 2).
 * Moi hang bao gom thong tin va cac nut hanh dong.
 */
struct PackageRowUI {
    HopDong* hopDong;      // Con tro den HopDong goc
    sf::Text info;         // Text hien thi (Ten goi, Ngay het han)
    Button btnGiaHan;
    Button btnDatHLV;
    PackageRowUI(sf::Font& font)
        : hopDong(nullptr),
          info(font, "", 16) // Khởi tạo 'info' ngay lập tức
    {
        // (btnGiaHan và btnDatHLV sẽ được tự động khởi tạo)
    }
};

/*
 * Lop HoiVienDetailPopup hien thi chi tiet thong tin cua mot Hoi Vien.
 * No su dung TabManager de chia thanh 2 tab theo yeu cau.
 */
class HoiVienDetailPopup : public BasePopup {
private:
    sf::Font& font;
    HoiVien* currentHoiVien; // Hoi vien dang duoc xem
    TabManager tabManager;

    // --- Tab 1: Thong Tin Ca Nhan ---
    std::vector<sf::Text> infoLines; // Danh sach cac dong text (VD: "Ten: Nguyen Van A")
    
    // --- Tab 2: Goi Tap Dang Ky ---
    std::vector<PackageRowUI> packageRows; // Danh sach cac goi tap UI
    Button btnDangKyGoiMoi; // Nut "Dang ky goi moi" o cuoi tab 2

    // --- Ham Helper ---
    /**
     * @brief Ve noi dung cho Tab 1 (Thong tin).
     */
    void drawTabInfo(sf::RenderTarget& target);
    
    /**
     * @brief Ve noi dung cho Tab 2 (Goi tap).
     */
    void drawTabPackages(sf::RenderTarget& target);

    /**
     * @brief Tao mot dong text theo dinh dang "Label: Value".
     */
    void createInfoLine(const std::string& label, const std::string& value, float yPos);
    
    /**
     * @brief Tai du lieu tu currentHoiVien vao cac component UI (infoLines, packageRows).
     */
    void loadData();

protected:
    /**
     * @brief Ghi de ham ao: Ve noi dung dua tren tab dang active.
     */
    void drawContent(sf::RenderTarget& target) override;

public:
    HoiVienDetailPopup(App& app);
    
    /**
     * @brief Hien thi popup voi thong tin cua mot Hoi Vien cu a.
     * @param hv Con tro den HoiVien can xem.
     */
    void show(HoiVien* hv);
    
    /**
     * @brief An popup va xoa du lieu tam.
     */
    void hide() override;

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
#pragma once
#include "BasePopup.h"
#include "TabManager.h"
#include "Button.h"
#include "HoiVien.h"
#include "HopDong.h"
#include "CartPopup.h" // ✅ THÊM INCLUDE
#include "DatHLVPopup.h" // ✅ THÊM INCLUDE
#include <vector>

// ... existing code ...

class HoiVienDetailPopup : public BasePopup {
private:
    sf::Font& font;
    TabManager tabManager;
    HoiVien* currentHoiVien;
    
    // Tab 1
    std::vector<sf::Text> infoLines;
    
    // Tab 2
    struct PackageRowUI {
        HopDong* hopDong;
        sf::Text info;
        Button btnGiaHan;
        Button btnDatHLV;
        
        PackageRowUI(sf::Font& f) 
            : hopDong(nullptr), info(f, "", 14), btnGiaHan(), btnDatHLV() {}
    };
    std::vector<PackageRowUI> packageRows;
    
    Button btnDangKyGoiMoi;
    
    // ✅ THÊM POPUPS
    CartPopup& cartPopup;
    DatHLVPopup& datHLVPopup;
    
    void loadData();
    void createInfoLine(const std::string& label, const std::string& value, float yPos);
    void drawTabInfo(sf::RenderTarget& target);
    void drawTabPackages(sf::RenderTarget& target);
    
protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    // ✅ SỬA CONSTRUCTOR
    HoiVienDetailPopup(App& app, CartPopup& cart, DatHLVPopup& hlv);
    
    void show(HoiVien* hv);
    void hide() override;
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
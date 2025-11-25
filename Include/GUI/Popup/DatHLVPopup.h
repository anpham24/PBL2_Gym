#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "HoiVien.h"
#include "MonTap.h"
#include "HLV.h"
#include "HopDong.h"
#include "MyVector.h"
#include "Config.h"
#include <functional> // ✅ THÊM

class DatHLVPopup : public BasePopup {
private:
    sf::Font& font;
    
    HoiVien* currentHoiVien;
    HopDong* currentHopDong;
    HLV* selectedHLV;
    
    std::function<void()> onCloseCallback; // ✅ THÊM
    
    MyVector<HLV*> allActiveHLV;
    MyVector<HLV*> displayedHLV;
    
    InputBox searchInput;
    sf::RectangleShape listContainer;
    int hoveredListIndex;
    sf::Text errorMessage;
    
    Button confirmButton;
    Button cancelButton;
    
    void loadAllHLV();
    void applySearchFilter();
    void handleSubmit();

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    DatHLVPopup(App& app);
    
    // ✅ SỬA: Thêm callback
    void show(HoiVien* hv, HopDong* hd, std::function<void()> onClose = nullptr);
    
    void hide() override;
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
// GUI/Popups/DatHLVPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "HoiVien.h"
#include "MonTap.h"
#include "HLV.h"
#include "MyVector.h"
#include <vector>
#include <string>

/*
 * (DA SUA DOI)
 * Lop DatHLVPopup hien thi TOAN BO danh sach HLV de Hoi Vien dat lich tap.
 * No co thanh tim kiem theo ten.
 */
class DatHLVPopup : public BasePopup {
private:
    HoiVien* currentHoiVien;
    HLV* selectedHLV;         // HLV duoc chon trong danh sach

    InputBox searchInput;
    Button confirmButton;
    Button cancelButton;
    sf::Text errorMessage;
    sf::Text tooltip; // De hien SDT khi hover

    // Danh sach HLV
    sf::RectangleShape listContainer; 
    MyVector<HLV*> allActiveHLV; // (MOI) Tat ca HLV dang hoat dong
    MyVector<HLV*> displayedHLV; // HLV sau khi loc = search
    int hoveredListIndex; 

    // Ham helper
    /**
     * @brief (MOI) Tai TOAN BO HLV tu QuanLy.
     */
    void loadAllHLV();
    
    /**
     * @brief Loc danh sach `allActiveHLV` dua vao `searchInput`.
     */
    void applySearchFilter();
    
    void handleSubmit();

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    DatHLVPopup(App& app);
    
    /**
     * @brief (DA SUA DOI) Hien thi popup de dat HLV.
     * @param hv Hoi vien dang dat lich.
     */
    void show(HoiVien* hv);
    void hide() override;

    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
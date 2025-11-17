// GUI/Popups/GoiTapMonTapPopup.h
#pragma once

#include "BasePopup.h"
#include "TabManager.h"
#include "InputBox.h"
#include "GoiTap.h"
#include "MonTap.h"
#include "MyVector.h"
#include <vector>
#include <string>

/*
 * Lop GoiTapMonTapPopup hien thi cac Mon Tap co trong Goi Tap.
 * No co 2 tab: "Mon Da Gan" và "Them Mon Tap".
 * No se an tab "Them Mon Tap" va nut "Xoa" neu user la Staff (read-only).
 */
class GoiTapMonTapPopup : public BasePopup {
private:
    GoiTap* currentGoiTap;
    TabManager tabManager;
    bool isReadOnly; // (MOI) Co de an chuc nang cho Staff

    // --- Tab 1: Mon Da Gan ---
    MyVector<MonTap*> assignedMonTap;
    int hoveredAssignedIndex;

    // --- Tab 2: Them Mon ---
    InputBox searchInput;
    MyVector<MonTap*> availableMonTap; // Mon tap (da loc) chua co trong goi
    int hoveredAvailableIndex;
    
    // Ham helper
    void loadData(); // Load data cho ca 2 tab
    void handleRemoveMonTap(MonTap* mt);
    void handleAddMonTap(MonTap* mt);
    
    // Ham ve
    void drawTabAssigned(sf::RenderTarget& target);
    void drawTabAdd(sf::RenderTarget& target);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    GoiTapMonTapPopup(App& app);
    
    /**
     * @brief Hien thi popup.
     * @param gt Goi tap can xem.
     * @param isReadOnly True neu la Staff (chi doc).
     */
    void show(GoiTap* gt, bool isReadOnly);
    void hide() override;

    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
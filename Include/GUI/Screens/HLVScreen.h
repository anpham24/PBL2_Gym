// GUI/Screens/HLVScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "HLVFormPopup.h"       // (Chung ta se tao)
#include "HLVDetailPopup.h"    // (Chung ta se tao)
#include "ConfirmDeletePopup.h" // (Tai su dung)
#include "HLV.h"

/*
 * Lop HLVScreen quan ly giao dien cho Huan Luyen Vien.
 * No kiem tra quyen Admin/Staff de an/hien nut Them/Sua/Xoa.
 */
class HLVScreen : public BaseScreen {
private:
    // Data
    MyVector<HLV*> allHLV;
    MyVector<HLV*> pagedHLV;

    // UI Components
    Table<HLV> hlvTable;
    Pagination pagination;
    Button themHLVButton;
    // (Bo loc/tim kiem)

    // Popups
    HLVFormPopup formPopup;
    HLVDetailPopup detailPopup;
    ConfirmDeletePopup deletePopup;

    bool isStaffReadOnly; // Co de luu trang thai Read-only

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    HLVScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
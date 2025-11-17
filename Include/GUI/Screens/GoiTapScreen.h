// GUI/Screens/GoiTapScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "GoiTapFormPopup.h"     // (Chung ta se tao)
#include "GoiTapMonTapPopup.h"  // (Chung ta se tao)
#include "ConfirmDeletePopup.h" // (Tai su dung)
#include "GoiTap.h"

/*
 * Lop GoiTapScreen quan ly giao dien cho Goi Tap.
 * No kiem tra quyen Admin/Staff de an/hien nut Them/Sua/Xoa.
 */
class GoiTapScreen : public BaseScreen {
private:
    // Data
    MyVector<GoiTap*> allGoiTap;    // Tat ca goi tap tu QuanLy
    MyVector<GoiTap*> pagedGoiTap;  // Goi tap sau khi phan trang

    // UI Components
    Table<GoiTap> goiTapTable;
    Pagination pagination;
    Button themGoiTapButton;
    // (Ban se them cac nut Filter/Sort o day)

    // Popups
    GoiTapFormPopup formPopup;
    GoiTapMonTapPopup monTapPopup;
    ConfirmDeletePopup deletePopup;

    bool isStaffReadOnly; // Co de luu trang thai Read-only

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    GoiTapScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
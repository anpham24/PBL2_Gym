// GUI/Screens/LopHocScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "LopHocFormPopup.h"     // (Chung ta se tao)
#include "ConfirmDeletePopup.h" // (Tai su dung)
#include "LopHoc.h"
#include "InputBox.h"

/*
 * Lop LopHocScreen quan ly giao dien cho Lop Hoc.
 * No kiem tra quyen Admin/Staff de an/hien nut Them/Sua/Xoa.
 */
class LopHocScreen : public BaseScreen {
private:
    // Data
    MyVector<LopHoc*> allLopHoc;
    MyVector<LopHoc*> pagedLopHoc;

    // UI Components
    Table<LopHoc> lopHocTable;
    Pagination pagination;
    Button themLopHocButton;
    // (Bo loc/tim kiem)

    // Popups
    LopHocFormPopup formPopup;
    ConfirmDeletePopup deletePopup;

    InputBox searchBox;
    void applySearch();

    bool isStaffReadOnly; // Co de luu trang thai Read-only

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    LopHocScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
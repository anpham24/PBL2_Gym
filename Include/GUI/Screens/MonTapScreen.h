// GUI/Screens/MonTapScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "MonTapFormPopup.h"     // (Chung ta se tao)
#include "ConfirmDeletePopup.h" // (Tai su dung tu HoiVien)
#include "MonTap.h"
#include "InputBox.h"

/*
 * Lop MonTapScreen quan ly giao dien cho Mon Tap.
 * No kiem tra quyen Admin/Staff de an/hien nut Them/Sua/Xoa.
 */
class MonTapScreen : public BaseScreen {
private:
    // Data
    MyVector<MonTap*> allMonTap;    // Tat ca mon tap tu QuanLy
    MyVector<MonTap*> pagedMonTap;  // Mon tap sau khi phan trang

    // UI Components
    Table<MonTap> monTapTable;
    Pagination pagination;
    Button themMonTapButton;

    // Popups
    MonTapFormPopup formPopup;
    ConfirmDeletePopup deletePopup;

    InputBox searchBox;
    void applySearch();

    bool isStaffReadOnly; // Co de luu trang thai Read-only

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    MonTapScreen(App& app);
    ~MonTapScreen(); // Ham huy (neu can)
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
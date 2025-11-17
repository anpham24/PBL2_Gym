// GUI/Screens/HangHoaScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "HangHoaFormPopup.h"   // (Chung ta se tao)
#include "ConfirmDeletePopup.h" // (Tai su dung)
#include "HangHoa.h"

/*
 * Lop HangHoaScreen quan ly giao dien cho Hang Hoa.
 * No kiem tra quyen Admin/Staff de an/hien nut Them/Sua/Xoa.
 */
class HangHoaScreen : public BaseScreen {
private:
    // Data
    MyVector<HangHoa*> allHangHoa;
    MyVector<HangHoa*> pagedHangHoa;

    // UI Components
    Table<HangHoa> hangHoaTable;
    Pagination pagination;
    Button themHangHoaButton;
    // (Bo loc/tim kiem)

    // Popups
    HangHoaFormPopup formPopup;
    ConfirmDeletePopup deletePopup;

    bool isStaffReadOnly; // Co de luu trang thai Read-only

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    HangHoaScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
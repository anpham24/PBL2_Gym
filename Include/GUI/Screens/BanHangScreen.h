// GUI/Screens/BanHangScreen.h
#pragma once

#include "BaseScreen.h"
#include "../App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "CartPopup.h"           // (Chung ta se tao)
#include "InvoiceDetailPopup.h" // (Chung ta se tao)
#include "ConfirmDeletePopup.h"   // (Tai su dung)
#include "HoaDon.h"

/*
 * Lop BanHangScreen hien thi LICH SU cac hoa don da thanh toan.
 * No cung cap nut "Them" de mo CartPopup va tao giao dich moi.
 */
class BanHangScreen : public BaseScreen {
private:
    // Data
    MyVector<HoaDon*> allPaidHoaDon;   // Tat ca hoa don da thanh toan
    MyVector<HoaDon*> pagedHoaDon;     // Da phan trang

    // UI Components
    Table<HoaDon> hoaDonTable; // Chi 1 bang lich su
    Pagination pagination;
    Button themHoaDonButton;
    // (Bo loc/tim kiem)

    // Popups
    CartPopup cartPopup;
    InvoiceDetailPopup detailPopup;
    ConfirmDeletePopup deletePopup;

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    BanHangScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
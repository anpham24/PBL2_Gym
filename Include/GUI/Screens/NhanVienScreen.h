// GUI/Screens/NhanVienScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "NhanVienFormPopup.h"   // (Chung ta se tao)
#include "NhanVienDetailPopup.h" // (Chung ta se tao)
#include "ConfirmDeletePopup.h"   // (Tai su dung)
#include "NhanVien.h"

/*
 * Lop NhanVienScreen quan ly giao dien cho Nhan Vien.
 * No kiem tra quyen Admin/Staff de an/hien nut Them/Sua/Xoa.
 */
class NhanVienScreen : public BaseScreen {
private:
    // Data
    MyVector<NhanVien*> allNhanVien;
    MyVector<NhanVien*> pagedNhanVien;

    // UI Components
    Table<NhanVien> nhanVienTable;
    Pagination pagination;
    Button themNhanVienButton;
    // (Bo loc/tim kiem)

    // Popups
    NhanVienFormPopup formPopup;
    NhanVienDetailPopup detailPopup;
    ConfirmDeletePopup deletePopup;

    bool isStaffReadOnly; // Co de luu trang thai Read-only

    // Ham helper
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    NhanVienScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
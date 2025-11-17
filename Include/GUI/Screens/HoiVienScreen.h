// GUI/Screens/HoiVienScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "HoiVienFormPopup.h"
#include "HoiVienDetailPopup.h"
#include "ConfirmDeletePopup.h"
#include "HoiVien.h"

/*
 * Lop HoiVienScreen la man hinh noi dung chinh de quan ly Hoi Vien.
 * No chua Table, Pagination, nut "Them", va 3 loai Popup.
 * No cung kiem tra quyen (Admin/Staff) de an/hien cac nut.
 */
class HoiVienScreen : public BaseScreen {
private:
    // Data
    MyVector<HoiVien*> allHoiVien;    // Tat ca hoi vien tu QuanLy
    MyVector<HoiVien*> pagedHoiVien;  // Hoi vien sau khi phan trang

    // UI Components
    Table<HoiVien> hoiVienTable;
    Pagination pagination;
    Button themHoiVienButton;
    // (Ban se them cac nut Filter/Sort o day)

    // Popups
    HoiVienFormPopup formPopup;
    HoiVienDetailPopup detailPopup;
    ConfirmDeletePopup deletePopup;

    bool isStaffReadOnly; // Cờ để lưu trạng thái Read-only của Staff

    /**
     * @brief Tai du lieu tu QuanLy, (sau nay se loc), va thiet lap pagination.
     */
    void loadAndDisplayData();
    
    /**
     * @brief Duoc goi boi Pagination de lay du lieu cho trang moi.
     */
    void onPageChange(int newPage);

public:
    HoiVienScreen(App& app);
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
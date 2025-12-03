#pragma once
#include "BaseScreen.h"
#include "Table.h"
#include "Pagination.h"
#include "Button.h"
#include "Inputbox.h"
#include "HoiVienFormPopup.h"
#include "HoiVienDetailPopup.h"
#include "ConfirmDeletePopup.h"
#include "CartPopup.h"        
#include "DatHLVPopup.h"      
#include "HoiVien.h"

class HoiVienScreen : public BaseScreen {
private:
    Table<HoiVien> hoiVienTable;
    Pagination pagination;
    Button themHoiVienButton;
    
    MyVector<HoiVien*> allHoiVien;
    MyVector<HoiVien*> pagedHoiVien;

    InputBox searchBox;
    void applySearch();
    
    bool isStaffReadOnly = false;
    
    // ✅ POPUPS - THỨ TỰ QUAN TRỌNG!
    HoiVienFormPopup formPopup;
    CartPopup cartPopup;           // ✅ Phải khai báo TRƯỚC detailPopup
    DatHLVPopup datHLVPopup;       // ✅ Phải khai báo TRƯỚC detailPopup
    HoiVienDetailPopup detailPopup; // ✅ Khai báo SAU
    ConfirmDeletePopup deletePopup;
    
    void loadAndDisplayData();
    void onPageChange(int newPage);

public:
    HoiVienScreen(App& app);
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
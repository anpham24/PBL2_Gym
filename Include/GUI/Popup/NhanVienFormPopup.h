// GUI/Popups/NhanVienFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "Checkbox.h" // <-- THEM CHECKBOX
#include "HoiVienFormPopup.h" // De tai su dung GenderSelector
#include "NhanVien.h"
#include <functional>

/*
 * Lop NhanVienFormPopup la popup de Them hoac Sua thong tin Nhan Vien.
 * No chua logic "Cap tai khoan" khi them moi.
 */
class NhanVienFormPopup : public BasePopup {
private:
    NhanVien* currentNhanVien; // Con tro den Nhan Vien dang sua
    std::function<void()> onSuccessCallback; // Callback de load lai bang

    // Form components
    InputBox hoTenInput;
    InputBox sdtInput;
    InputBox ngaySinhInput;
    GenderSelector genderSelector;
    InputBox luongInput;
    Checkbox capTaiKhoanCheck; // <-- CHECKBOX MOI
    
    sf::Text errorMessage;
    Button confirmButton;
    Button cancelButton;

    int focusIndex; // De dieu huong phim

    /**
     * @brief Kiem tra input, su dung Validator.
     * @return true neu hop le.
     */
    bool validate(std::string& ten, std::string& sdt, std::string& gioiTinh, 
                  std::string& ngaySinh, double& luong);
    
    void handleSubmit();
    void updateFocus();
    void handleKeyNavigation(sf::Keyboard::Key key);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    NhanVienFormPopup(App& app);
    
    /**
     * @brief Hien thi form.
     * @param nv Con tro den NhanVien de SUA (nullptr neu THEM MOI).
     * @param onSuccess Callback de goi khi thanh cong.
     */
    void show(NhanVien* nv, std::function<void()> onSuccess);

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
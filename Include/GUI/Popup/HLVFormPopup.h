// GUI/Popups/HLVFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "HoiVienFormPopup.h" // De tai su dung GenderSelector
#include "HLV.h"
#include <functional>

/*
 * Lop HLVFormPopup la popup de Them hoac Sua thong tin HLV.
 * No ke thua BasePopup.
 */
class HLVFormPopup : public BasePopup {
private:
    HLV* currentHLV; // Con tro den HLV dang sua (nullptr neu la them moi)
    
    // Ham callback se duoc goi khi Them/Sua thanh cong (de load lai bang)
    std::function<void()> onSuccessCallback;

    // Form components
    InputBox hoTenInput;
    InputBox sdtInput;
    InputBox ngaySinhInput;
    GenderSelector genderSelector;
    InputBox chuyenMonInput; // Truong string (thay vi vector<MonTap>)
    InputBox luongInput;
    
    sf::Text errorMessage;
    Button confirmButton;
    Button cancelButton;

    int focusIndex; // De dieu huong phim

    /**
     * @brief Kiem tra input, su dung Validator.
     * @return true neu hop le.
     */
    bool validate(std::string& ten, std::string& sdt, std::string& gioiTinh, 
                  std::string& ngaySinh, std::string& chuyenMon, double& luong);
    
    void handleSubmit();
    void updateFocus();
    void handleKeyNavigation(sf::Keyboard::Key key);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    HLVFormPopup(App& app);
    
    /**
     * @brief Hien thi form.
     * @param hlv Con tro den HLV de SUA (nullptr neu THEM MOI).
     * @param onSuccess Callback de goi khi thanh cong.
     */
    void show(HLV* hlv, std::function<void()> onSuccess);

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
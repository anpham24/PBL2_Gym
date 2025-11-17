// GUI/Popups/LopHocFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "Selector.h" // <-- SỬ DỤNG SELECTOR
#include "LopHoc.h"
#include "MonTap.h"
#include "HLV.h"
#include <functional>

/*
 * Lop LopHocFormPopup la popup de Them/Sua Lop Hoc.
 * No chua logic phu thuoc (dependent logic) giua MonTap va HLV.
 */
class LopHocFormPopup : public BasePopup {
private:
    LopHoc* currentLopHoc;
    std::function<void()> onSuccessCallback;

    // Form components
    InputBox tenLopInput;
    InputBox lichTapInput; // VD: "T4-18:00"
    InputBox thoiLuongInput; // "60"
    
    Selector<MonTap> monTapSelector;
    Selector<HLV> hlvSelector;
    
    sf::Text errorMessage;
    Button confirmButton;
    Button cancelButton;
    
    /**
     * @brief Tai toan bo data MonTap va HLV vao Selectors.
     */
    void loadSelectors();
    
    /**
     * @brief Kiem tra input, su dung Validator.
     * @return true neu hop le.
     */
    bool validate(std::string& ten, std::string& lich, int& thoiLuong, MonTap*& mt, HLV*& hlv);
    
    void handleSubmit();
    // (Chung ta se don gian hoa, khong can dieu huong phim cho form phuc tap nay)

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    LopHocFormPopup(App& app);
    
    void show(LopHoc* lh, std::function<void()> onSuccess);

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
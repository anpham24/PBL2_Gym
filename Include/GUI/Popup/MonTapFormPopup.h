// GUI/Popups/MonTapFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "MonTap.h"
#include <functional>

/*
 * Lop MonTapFormPopup la mot popup don gian de Them hoac Sua Ten Mon Tap.
 * No ke thua BasePopup.
 */
class MonTapFormPopup : public BasePopup {
private:
    MonTap* currentMonTap; // Con tro den Mon Tap dang sua (nullptr neu la them moi)
    
    // Ham callback se duoc goi khi Them/Sua thanh cong (de load lai bang)
    std::function<void()> onSuccessCallback;

    // Form components
    InputBox tenMonInput;
    Button confirmButton;
    Button cancelButton;
    sf::Text errorMessage; // De hien thi loi tu Validator

    /**
     * @brief Kiem tra input, su dung Validator.
     * @param tenMon (Out) Bien de luu ten mon neu hop le.
     * @return true neu hop le.
     */
    bool validate(std::string& tenMon);
    
    /**
     * @brief Goi khi nhan "Xac Nhan", se goi validate() va Service.
     */
    void handleSubmit();

protected:
    /**
     * @brief Ve noi dung (InputBox, Buttons, Error) len panel.
     */
    void drawContent(sf::RenderTarget& target) override;

public:
    MonTapFormPopup(App& app);
    
    /**
     * @brief Hien thi form.
     * @param mt Con tro den MonTap de SUA (nullptr neu THEM MOI).
     * @param onSuccess Callback de goi khi thanh cong.
     */
    void show(MonTap* mt, std::function<void()> onSuccess);

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
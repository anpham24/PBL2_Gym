// GUI/Popups/GoiTapFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "GoiTap.h"
#include <functional>
#include <vector>

/*
 * (Component ThoiGianSelector da duoc XOA)
 */

/*
 * Lop GoiTapFormPopup la popup de Them hoac Sua thong tin Goi Tap.
 */
class GoiTapFormPopup : public BasePopup {
private:
    GoiTap* currentGoiTap;
    std::function<void()> onSuccessCallback;

    // Form components
    InputBox tenGoiInput;
    InputBox thoiGianInput; // <-- (MOI) Thay the cho ThoiGianSelector
    InputBox giaInput;
    InputBox soBuoiPTInput;
    
    sf::Text errorMessage;
    Button confirmButton;
    Button cancelButton;
    
    int focusIndex; // De dieu huong phim

    /**
     * @brief Kiem tra input, su dung Validator.
     * @return true neu hop le.
     */
    bool validate(std::string& ten, int& thoiGian, double& gia, int& soBuoiPT);
    
    void handleSubmit();
    void updateFocus();
    void handleKeyNavigation(sf::Keyboard::Key key);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    GoiTapFormPopup(App& app);
    
    void show(GoiTap* gt, std::function<void()> onSuccess);
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
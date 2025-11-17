// GUI/Popups/HangHoaFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "HangHoa.h"
#include <functional>
#include <string>
#include <vector>

/*
 * (Component con)
 * Dinh nghia mot nhom 3 nut de chon Loai Hang Hoa.
 */
class LoaiHHSelector {
public:
    sf::Text label;
    Button btnThucPham;
    Button btnDungCu;
    Button btnKhac;
    std::string selectedLoai; // "Thuc pham", "Dung cu", "Khac"
    sf::Font& font;
    
    LoaiHHSelector(sf::Font& font);
    void setup(float x, float y);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    void setSelected(const std::string& loai);
    void setFocused(bool focus1, bool focus2, bool focus3);
};


/*
 * Lop HangHoaFormPopup la popup de Them hoac Sua thong tin Hang Hoa.
 */
class HangHoaFormPopup : public BasePopup {
private:
    HangHoa* currentHangHoa; // Con tro den Hang Hoa dang sua
    std::function<void()> onSuccessCallback; // Callback de load lai bang

    // Form components
    InputBox tenHHInput;
    LoaiHHSelector loaiSelector;
    InputBox giaInput;
    InputBox soLuongInput;
    
    sf::Text errorMessage;
    Button confirmButton;
    Button cancelButton;

    int focusIndex; // De dieu huong phim

    /**
     * @brief Kiem tra input, su dung Validator.
     * @return true neu hop le.
     */
    bool validate(std::string& ten, std::string& loai, double& gia, int& soLuong);
    
    void handleSubmit();
    void updateFocus();
    void handleKeyNavigation(sf::Keyboard::Key key);

protected:
    void drawContent(sf::RenderTarget& target) override;

public:
    HangHoaFormPopup(App& app);
    
    /**
     * @brief Hien thi form.
     * @param hh Con tro den HangHoa de SUA (nullptr neu THEM MOI).
     * @param onSuccess Callback de goi khi thanh cong.
     */
    void show(HangHoa* hh, std::function<void()> onSuccess);

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
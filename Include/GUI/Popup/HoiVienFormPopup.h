// GUI/Popups/HoiVienFormPopup.h
#pragma once

#include "BasePopup.h"
#include "InputBox.h"
#include "Button.h"
#include "HoiVien.h"
#include <functional> // De su dung std::function

/*
 * (Component con)
 * Dinh nghia mot nhom 2 nut (Nam/Nu) de chon gioi tinh.
 */
class GenderSelector {
public:
    sf::Text label;
    Button btnNam;
    Button btnNu;
    int selectedGender; // 0 = chua chon, 1 = Nam, 2 = Nu
    sf::Font& font;

    GenderSelector(sf::Font& font);
    void setup(float x, float y);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    std::string getString();
    void setFocused(bool focusNam, bool focusNu);
};


/*
 * Lop HoiVienFormPopup la popup chinh de Them hoac Sua thong tin Hoi Vien.
 * No ke thua BasePopup va chua toan bo logic form.
 */
class HoiVienFormPopup : public BasePopup {
private:
    HoiVien* currentHoiVien; // Con tro den hoi vien dang sua (nullptr neu la them moi)

    // Ham callback se duoc goi khi Them/Sua thanh cong (de load lai bang)
    std::function<void()> onSuccessCallback; 

    // Form components
    InputBox hoTenInput;
    InputBox sdtInput;
    InputBox ngaySinhInput;
    GenderSelector genderSelector;
    sf::Text errorMessage; // De hien "Vui long nhap du thong tin"

    Button confirmButton;
    Button cancelButton;

    // Dinh vi focus de dieu huong bang phim
    int focusIndex; // 0=HoTen, 1=SDT, 2=NgaySinh, 3=Nam, 4=Nu, 5=Confirm, 6=Cancel

    // Ham xu ly
    /**
     * @brief Kiem tra tat ca input, su dung Validator.
     * @return true neu tat ca deu hop le.
     */
    bool validateInfo(std::string& ten, std::string& sdt, std::string& gioiTinh, std::string& ngaySinh);
    
    /**
     * @brief Goi khi nhan "Xac Nhan", se goi validateInfo va Service.
     */
    void handleSubmit();
    
    /**
     * @brief Cap nhat giao dien focus (vien vang) cho component.
     */
    void updateFocus();
    
    /**
     * @brief Xu ly dieu huong bang phim (Tab, Enter, Mui ten).
     */
    void handleKeyNavigation(sf::Keyboard::Key key);

protected:
    // Ve cac input, button vao popupPanel
    void drawContent(sf::RenderTarget& target) override;

public:
    HoiVienFormPopup(App& app);
    
    // An/Hien
    /**
     * @brief Hien thi form.
     * @param hv Con tro den HoiVien de SUA (nullptr neu THEM MOI).
     * @param onSuccess Callback de goi khi thanh cong.
     */
    void show(HoiVien* hv, std::function<void()> onSuccess);

    // Ghi de cac ham cua BasePopup
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
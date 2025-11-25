#pragma once

#include "BasePopup.h"
#include "HoiVien.h"
#include "InputBox.h"
#include "Button.h"
#include <functional>

// Component con de chon gioi tinh
struct GenderSelector {
    sf::Font& font;
    int selectedGender; // 0: None, 1: Nam, 2: Nu
    
    sf::Text* label; // <--- QUAN TRỌNG: Phải là CON TRỎ
    
    Button btnNam;
    Button btnNu;

    GenderSelector(sf::Font& font);
    ~GenderSelector();

    // --- QUAN TRỌNG: Copy Constructor & Assignment ---
    GenderSelector(const GenderSelector& other);
    GenderSelector& operator=(const GenderSelector& other);

    void setup(float x, float y);
    void draw(sf::RenderTarget& target);
    void handleEvent(sf::Event event, sf::Vector2i mousePos);
    void update(sf::Vector2i mousePos);
    
    std::string getString();
    void setFocused(bool focusNam, bool focusNu);
};

class HoiVienFormPopup : public BasePopup {
private:
    HoiVien* currentHoiVien; // Neu null -> Them moi, nguoc lai -> Sua
    
    // Cac truong nhap lieu
    InputBox hoTenInput;
    InputBox sdtInput;
    InputBox ngaySinhInput;
    
    GenderSelector genderSelector;
    int focusIndex; // De dieu huong bang tab/enter
    sf::Text errorMessage; // Thong bao loi
    
    Button confirmButton;
    Button cancelButton;
    
    
    std::function<void()> onSuccessCallback;

    bool validateInfo(std::string& ten, std::string& sdt, std::string& gioiTinh, std::string& ngaySinh);
    void handleSubmit();
    
    void updateFocus();
    void handleKeyNavigation(sf::Keyboard::Key key);

public:
    HoiVienFormPopup(App& app);
    
    void show(HoiVien* hv = nullptr, std::function<void()> onSuccess = nullptr);
    
    // Override tu BasePopup
    void drawContent(sf::RenderTarget& target) override;
    void handleEvent(sf::Event event, sf::Vector2i mousePos) override;
    void update(sf::Vector2i mousePos) override;
};
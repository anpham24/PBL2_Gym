// GUI/Screens/LoginScreen.h
#pragma once

#include "BaseScreen.h"
#include "App.h" // De goi app.changeScreen
#include "InputBox.h"
#include "Button.h"
#include "Checkbox.h"

// Dinh nghia cac thanh phan co the duoc focus (chon)
enum class LoginFocus {
    USERNAME,
    PASSWORD,
    SHOW_PASS,
    LOGIN_BTN,
    EXIT_BTN
};

class LoginScreen : public BaseScreen {
private:
    sf::Text* title;
    sf::Text* errorText;

    InputBox usernameInput;
    InputBox passwordInput;
    Checkbox showPasswordCheck;
    Button loginButton;
    Button exitButton;

    LoginFocus currentFocus; // Thanh phan dang duoc chon hien tai

    // Ham xu ly logic dang nhap
    void handleLogin();
    
    // Ham xu ly dieu huong bang ban phim (Tab, Mui ten)
    void handleKeyNavigation(sf::Keyboard::Key key);
    
    // Cap nhat giao dien khi focus thay doi
    void updateFocusVisuals();

public:
    /**
     * @brief Ham dung.
     * @param app Tham chieu den App chinh.
     */
    LoginScreen(App& app);
    ~LoginScreen();
    
    void handleEvent(sf::Event event) override;
    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target) override;
};
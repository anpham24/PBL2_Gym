#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "BaseScreen.h"
#include "../Manager/LoginSystem.h"
#include "../UI/ModernInputBox.h" 
#include "../UI/ModernButton.h"  
#include <vector>
#include <memory>
#include <string>

// Sử dụng namespace cho các kiểu dữ liệu phổ biến
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

class LoginScreen : public BaseScreen {
private:
    sf::Font& m_font;
    LoginSystem& m_loginSystem; 

    // UI Components
    sf::RectangleShape m_loginPanel;
    
    // SỬA LỖI SFML 3: sf::Text phải được khởi tạo với một Font
    sf::Text m_loginTitleText;
    sf::Text m_defaultMsgText;
    sf::Text m_messageText;
    
    vector<unique_ptr<ModernInputBox>> m_inputs;
    unique_ptr<ModernButton> m_loginButton;

    // Trạng thái
    bool m_isLoginSuccessful;
    string m_loggedInUsername;

    void setupUI();
    void attemptLogin();
    void setMessage(const string& msg, sf::Color color);

public:
    // SỬA LỖI SFML 3: Thêm (Font& font) vào constructor
    LoginScreen(sf::Font& font, LoginSystem& loginSystem);
    virtual ~LoginScreen() {}

    // Kế thừa từ BaseScreen
    void handleEvent(sf::Event& event, const sf::Vector2f& mousePos) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    // Hàm lấy trạng thái
    bool isLoginSuccessful() const;
    string getLoggedInUser() const;
    void reset();
};

#endif // LOGINSCREEN_H
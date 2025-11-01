#include "LoginScreen.h"

// Sử dụng namespace cho gọn
using namespace sf;
using namespace std;

// SỬA LỖI SFML 3: Khởi tạo các biến sf::Text trong danh sách khởi tạo
LoginScreen::LoginScreen(Font& font, LoginSystem& loginSystem)
    : m_font(font), 
      m_loginSystem(loginSystem), 
      m_isLoginSuccessful(false),
      m_loginTitleText(m_font),   // <-- SỬA LỖI
      m_defaultMsgText(m_font),   // <-- SỬA LỖI
      m_messageText(m_font)       // <-- SỬA LỖI
{
    setupUI();
}

void LoginScreen::setupUI() {
    // Panel đăng nhập
    m_loginPanel.setSize({450, 400});
    m_loginPanel.setPosition({475, 250});
    m_loginPanel.setFillColor(Color(255, 255, 255));
    m_loginPanel.setOutlineColor(Color(200, 200, 200));
    m_loginPanel.setOutlineThickness(2);
    
    // Tiêu đề của Panel
    m_loginTitleText.setString("DANG NHAP HE THONG");
    m_loginTitleText.setCharacterSize(28);
    m_loginTitleText.setFillColor(Color(70, 130, 180));
    m_loginTitleText.setStyle(Text::Bold);
    m_loginTitleText.setPosition({550, 280});
    
    // Tin nhắn tài khoản mặc định
    m_defaultMsgText.setString("Tai khoan mac dinh: admin/admin123 hoac staff/staff123");
    m_defaultMsgText.setCharacterSize(14);
    m_defaultMsgText.setFillColor(Color(150, 150, 150));
    m_defaultMsgText.setPosition({475, 670});

    // Tin nhắn (lỗi/thành công)
    m_messageText.setCharacterSize(16);
    m_messageText.setPosition({475, 700}); // Vị trí cho tin nhắn động

    // Inputs
    m_inputs.clear();
    m_inputs.push_back(make_unique<ModernInputBox>(
        m_font, "Ten dang nhap", "Nhap ten dang nhap...", 525, 350));
    m_inputs.push_back(make_unique<ModernInputBox>(
        m_font, "Mat khau", "Nhap mat khau...", 525, 450, 350, 50, true)); // true = isPassword
    
    if (!m_inputs.empty()) {
        m_inputs[0]->setActive(true);
    }

    // Nút
    m_loginButton = make_unique<ModernButton>(m_font, "DANG NHAP", 575, 550, 250, 55);
}

void LoginScreen::handleEvent(Event& event, const Vector2f& mousePos) {
    // Xử lý UI
    for (auto& input : m_inputs) {
        input->handleEvent(event);
    }
    m_loginButton->handleEvent(event, mousePos);
    
    // Xử lý click
    if (m_loginButton->isClicked(event, mousePos)) {
        attemptLogin();
    }

    // SỬA LỖI SFML 3: Thay đổi cách xử lý sự kiện
    if (auto* keyEvent = event.getIf<Event::KeyPressed>()) {
        if (keyEvent->code == Keyboard::Key::Enter) {
            for (size_t i = 0; i < m_inputs.size(); i++) {
                if (m_inputs[i]->getIsActive()) {
                    m_inputs[i]->setActive(false);
                    
                    if (i + 1 < m_inputs.size()) {
                        m_inputs[i + 1]->setActive(true);
                    } else {
                        attemptLogin(); // Input cuối cùng, thực hiện đăng nhập
                    }
                    break;
                }
            }
        } else if (keyEvent->code == Keyboard::Key::Tab || keyEvent->code == Keyboard::Key::Down) {
            for (size_t i = 0; i < m_inputs.size(); i++) {
                if (m_inputs[i]->getIsActive()) {
                    m_inputs[i]->setActive(false);
                    size_t nextIndex = (i + 1) % m_inputs.size();
                    m_inputs[nextIndex]->setActive(true);
                    break;
                }
            }
        } else if (keyEvent->code == Keyboard::Key::Up) {
            for (size_t i = 0; i < m_inputs.size(); i++) {
                if (m_inputs[i]->getIsActive()) {
                    m_inputs[i]->setActive(false);
                    size_t prevIndex = (i == 0) ? m_inputs.size() - 1 : i - 1;
                    m_inputs[prevIndex]->setActive(true);
                    break;
                }
            }
        }
    }
}

void LoginScreen::update(float dt) {
    for (auto& input : m_inputs) {
        input->update(dt);
    }
}

void LoginScreen::draw(RenderWindow& window) {
    // Tiêu đề header (trong Application) không được vẽ ở đây
    
    window.draw(m_loginPanel);
    window.draw(m_loginTitleText);
    
    for (auto& input : m_inputs) {
        input->draw(window);
    }
    
    m_loginButton->draw(window);
    window.draw(m_defaultMsgText);
    window.draw(m_messageText);
}

void LoginScreen::attemptLogin() {
    string username = m_inputs[0]->getText();
    string password = m_inputs[1]->getText();

    if (username.empty() || password.empty()) {
        setMessage("Vui long nhap day du thong tin!", Color(220, 80, 80));
        return;
    }

    if (m_loginSystem.login(username, password)) {
        setMessage("Dang nhap thanh cong!", Color(80, 180, 80));
        m_isLoginSuccessful = true;
        m_loggedInUsername = username;
    } else {
        setMessage("Sai ten dang nhap hoac mat khau!", Color(220, 80, 80));
        m_isLoginSuccessful = false;
    }
}

void LoginScreen::setMessage(const string& msg, Color color) {
    m_messageText.setString(msg);
    m_messageText.setFillColor(color);
}

// ----- Các hàm lấy trạng thái -----

bool LoginScreen::isLoginSuccessful() const {
    return m_isLoginSuccessful;
}

string LoginScreen::getLoggedInUser() const {
    return m_loggedInUsername;
}

void LoginScreen::reset() {
    m_isLoginSuccessful = false;
    m_loggedInUsername = "";
    m_messageText.setString("");
    for (auto& input : m_inputs) {
        input->clear();
    }
    if (!m_inputs.empty()) {
        m_inputs[0]->setActive(true);
        if (m_inputs.size() > 1) {
            m_inputs[1]->setActive(false);
        }
    }
}
// GUI/Screens/LoginScreen.cpp
#include "LoginScreen.h"
#include <iostream>

LoginScreen::LoginScreen(App& app) 
    : BaseScreen(app), title(nullptr), errorText(nullptr) 
{
    sf::Font& font = app.getGlobalFont();
    sf::Vector2u windowSize = app.getWindow().getSize();
    float centerX = windowSize.x / 2.0f;

    // --- Title ---
    // SỬA Ở ĐÂY: Dùng 'new'
    title = new sf::Text(font, "HE THONG QUAN LY GYM", 32);
    title->setFillColor(Config::AccentCyan);
    title->setStyle(sf::Text::Bold);

    // SỬA Ở ĐÂY: Dùng '->'
    sf::FloatRect titleRect = title->getLocalBounds();
    title->setOrigin(sf::Vector2f(
        titleRect.position.x + titleRect.size.x / 2.0f,
        titleRect.position.y + titleRect.size.y / 2.0f
    ));
    title->setPosition(sf::Vector2f(centerX, 150));

    // --- Error Text ---
    // SỬA Ở ĐÂY: Dùng 'new'
    errorText = new sf::Text(font, "", 16);
    errorText->setFillColor(Config::Danger);
    errorText->setPosition(sf::Vector2f(centerX - 150, 430));

    // --- Input Fields ---
    usernameInput.setup("Tai khoan", font, false);
    usernameInput.setSize(300, 40);
    usernameInput.setPosition(centerX - 150, 220);

    passwordInput.setup("Mat khau", font, true); // Mac dinh la password
    passwordInput.setSize(300, 40);
    passwordInput.setPosition(centerX - 150, 280);

    // --- Checkbox ---
    showPasswordCheck.setup("Hien mat khau", font);
    showPasswordCheck.setPosition(centerX - 150, 335);
    
    // Logic: Khi tich vao -> Tat che do password (hien chu)
    // Khi bo tich -> Bat che do password (hien dau *)
    showPasswordCheck.setOnToggle([this](bool checked) {
        passwordInput.setPasswordMode(!checked); 
    });

    // --- Buttons ---
    loginButton.setup("Dang Nhap", font);
    loginButton.setSize(140, 45);
    loginButton.setPosition(centerX - 150, 370);
    loginButton.setOnClick([this]() {
        handleLogin();
    });

    exitButton.setup("Thoat", font);
    exitButton.setSize(140, 45);
    exitButton.setPosition(centerX + 10, 370);
    // Doi mau nut Thoat thanh mau do
    exitButton.setColors(Config::Danger, sf::Color(255, 100, 100));
    exitButton.setOnClick([this]() {
        this->app.stop(); // Goi ham dung App
    });

    // --- Thiet lap focus ban dau ---
    currentFocus = LoginFocus::USERNAME;
    updateFocusVisuals();
}

LoginScreen::~LoginScreen() {
    // Giai phong tai nguyen neu can
    delete title;
    delete errorText;
}

void LoginScreen::handleEvent(sf::Event event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    // 1. Xu ly su kien chuot (cho cac nut & checkbox)
    loginButton.handleEvent(event, mousePos);
    exitButton.handleEvent(event, mousePos);
    showPasswordCheck.handleEvent(event, mousePos);

    // 2. Xu ly click vao Input de chuyen Focus
    // (SFML 3.0: Dung getIf)
    if (auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            if (usernameInput.isMouseOver(mousePos)) {
                currentFocus = LoginFocus::USERNAME;
            } else if (passwordInput.isMouseOver(mousePos)) {
                currentFocus = LoginFocus::PASSWORD;
            }
            updateFocusVisuals();
        }
    }

    // 3. Xu ly nhap lieu (TextEntered) -> Gui cho Input dang focus
    if (event.getIf<sf::Event::TextEntered>()) {
        if (currentFocus == LoginFocus::USERNAME) {
            usernameInput.handleEvent(event);
        } else if (currentFocus == LoginFocus::PASSWORD) {
            passwordInput.handleEvent(event);
        }
    }

    // 4. Xu ly ban phim (Navigation & Actions)
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        // Truoc tien, gui phim sang InputBox de no xu ly (Left/Right/Backspace)
        if (currentFocus == LoginFocus::USERNAME) {
            usernameInput.handleEvent(event);
        } else if (currentFocus == LoginFocus::PASSWORD) {
            passwordInput.handleEvent(event);
        }
        
        // Sau do xu ly dieu huong (Tab, Enter, Mui ten Len/Xuong)
        handleKeyNavigation(keyEvent->code);
    }
}

void LoginScreen::handleKeyNavigation(sf::Keyboard::Key key) {
    // Dieu huong (Tab, Down) -> Di xuong
    if (key == sf::Keyboard::Key::Tab || key == sf::Keyboard::Key::Down) {
        switch (currentFocus) {
        case LoginFocus::USERNAME: currentFocus = LoginFocus::PASSWORD; break;
        case LoginFocus::PASSWORD: currentFocus = LoginFocus::SHOW_PASS; break;
        case LoginFocus::SHOW_PASS: currentFocus = LoginFocus::LOGIN_BTN; break;
        case LoginFocus::LOGIN_BTN: currentFocus = LoginFocus::EXIT_BTN; break;
        case LoginFocus::EXIT_BTN: currentFocus = LoginFocus::USERNAME; break;
        }
        updateFocusVisuals();
    }
    // Dieu huong (Up) -> Di len
    else if (key == sf::Keyboard::Key::Up) {
        switch (currentFocus) {
        case LoginFocus::USERNAME: currentFocus = LoginFocus::EXIT_BTN; break;
        case LoginFocus::PASSWORD: currentFocus = LoginFocus::USERNAME; break;
        case LoginFocus::SHOW_PASS: currentFocus = LoginFocus::PASSWORD; break;
        case LoginFocus::LOGIN_BTN: currentFocus = LoginFocus::SHOW_PASS; break;
        case LoginFocus::EXIT_BTN: currentFocus = LoginFocus::LOGIN_BTN; break;
        }
        updateFocusVisuals();
    }
    // Xu ly (Enter)
    else if (key == sf::Keyboard::Key::Enter) {
        switch (currentFocus) {
        case LoginFocus::USERNAME: // Enter o username -> xuong password
            currentFocus = LoginFocus::PASSWORD;
            updateFocusVisuals();
            break;
        case LoginFocus::PASSWORD: // Enter o password -> DANG NHAP
            handleLogin();
            break;
        case LoginFocus::SHOW_PASS: // Enter o checkbox -> Toggle
            showPasswordCheck.toggle();
            break;
        case LoginFocus::LOGIN_BTN: // Enter o nut Login -> Click
            loginButton.click();
            break;
        case LoginFocus::EXIT_BTN: // Enter o nut Exit -> Click
            exitButton.click();
            break;
        }
    }
}

void LoginScreen::updateFocusVisuals() {
    // Tat focus tat ca
    usernameInput.setFocus(false);
    passwordInput.setFocus(false);
    showPasswordCheck.setFocused(false);
    loginButton.setFocused(false);
    exitButton.setFocused(false);

    // Bat focus cho phan tu hien tai
    switch (currentFocus) {
    case LoginFocus::USERNAME:  usernameInput.setFocus(true); break;
    case LoginFocus::PASSWORD:  passwordInput.setFocus(true); break;
    case LoginFocus::SHOW_PASS: showPasswordCheck.setFocused(true); break;
    case LoginFocus::LOGIN_BTN: loginButton.setFocused(true); break;
    case LoginFocus::EXIT_BTN:  exitButton.setFocused(true); break;
    }
}

void LoginScreen::update(sf::Time dt) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(app.getWindow());

    usernameInput.update(dt);
    passwordInput.update(dt);
    showPasswordCheck.update(mousePos);
    loginButton.update(mousePos);
    exitButton.update(mousePos);
}

void LoginScreen::draw(sf::RenderTarget& target) {
    // SỬA Ở ĐÂY:
    if (title) target.draw(*title);

    usernameInput.draw(target);
    passwordInput.draw(target);
    showPasswordCheck.draw(target);
    loginButton.draw(target);
    exitButton.draw(target);

    // SỬA Ở ĐÂY:
    if (errorText) target.draw(*errorText);
}

void LoginScreen::handleLogin() {
    std::string user = usernameInput.getString();
    std::string pass = passwordInput.getString();

    if (user.empty() || pass.empty()) {
        if (errorText) errorText->setString("Vui long nhap day du thong tin"); // SỬA
        return;
    }

    // Goi AccountManager de kiem tra
    Account* acc = app.getAccountManager().validate(user, pass);

    if (acc != nullptr) {
        // Dang nhap thanh cong
       if (errorText) errorText->setString(""); 
        app.login(acc); // Luu phien dang nhap
        app.changeScreen(ScreenType::Main); // Chuyen sang man hinh chinh
    } else {
        // That bai
        if (errorText) errorText->setString("Tai khoan hoac mat khau khong dung");
    }
}
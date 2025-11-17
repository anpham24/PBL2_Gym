// GUI/Popups/ConfirmDeletePopup.cpp
#include "ConfirmDeletePopup.h"
#include <iostream>

ConfirmDeletePopup::ConfirmDeletePopup(App& app)
    : BasePopup(app, "Xac Nhan Xoa") ,
     mainMessage(font, "", 16),
     warningMessage(font, "", 14),
     confirmButton(),
     cancelButton()
{
    // Dieu chinh lai kich thuoc panel cho phu hop
    sf::Vector2f panelSize(450, 250);
    popupPanel.setSize(panelSize);
    
    // Dat lai vi tri panel (de canh giua voi kich thuoc moi)
    sf::Vector2u windowSize = app.getWindow().getSize();
    popupPanel.setPosition(sf::Vector2f(
        (windowSize.x - panelSize.x) / 2.0f,
        (windowSize.y - panelSize.y) / 2.0f
    ));
    
    // Dat lai vi tri title va nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(
        popupPanel.getPosition().x + panelSize.x - 40, 
        popupPanel.getPosition().y + 15
    );

    // --- Setup cac thanh phan cua popup ---
    
    // Thong bao chinh
    // mainMessage.setFont(font);
    // mainMessage.setCharacterSize(16);
    mainMessage.setFillColor(Config::TextNormal);
    mainMessage.setPosition(sf::Vector2f(
        popupPanel.getPosition().x + 30, 
        popupPanel.getPosition().y + 80
    ));

    // Thong bao canh bao (mau do)
    // warningMessage.setFont(font);
    // warningMessage.setCharacterSize(14);
    warningMessage.setFillColor(Config::Danger);
    warningMessage.setPosition(sf::Vector2f(
        popupPanel.getPosition().x + 30, 
        popupPanel.getPosition().y + 120
    ));

    // --- Setup cac nut ---
    float btnY = popupPanel.getPosition().y + panelSize.y - 70; // Cach day 70px
    
    // Nut Xac Nhan
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(popupPanel.getPosition().x + 70, btnY);
    // Doi mau nut Xac Nhan thanh mau do
    confirmButton.setColors(Config::Danger, sf::Color(255, 100, 100));
    confirmButton.setOnClick([this]() {
        if (onConfirmCallback) {
            onConfirmCallback(); // Goi ham callback da luu
        }
        this->hide(); // Dong popup
    });

    // Nut Huy
    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(popupPanel.getPosition().x + 260, btnY);
    // Mau mac dinh (AccentCyan)
    cancelButton.setOnClick([this]() {
        this->hide(); // Chi can dong popup
    });
}

void ConfirmDeletePopup::show(const std::string& itemName, std::function<void()> onConfirm, const std::string& warning) {
    // Cap nhat noi dung cho message
    std::string mainMsg = "Ban co chac chan muon xoa " + itemName + "?";
    mainMessage.setString(sf::String::fromUtf8(mainMsg.begin(), mainMsg.end()));
    
    warningMessage.setString(sf::String::fromUtf8(warning.begin(), warning.end()));
    
    // Luu lai ham callback de goi khi nhan Xac Nhan
    onConfirmCallback = onConfirm;     
    
    BasePopup::show(); // Hien thi popup
}

void ConfirmDeletePopup::drawContent(sf::RenderTarget& target) {
    // Ve cac thanh phan da thiet lap
    target.draw(mainMessage);
    target.draw(warningMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void ConfirmDeletePopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    
    // 1. Xu ly nut [X] (tu lop cha)
    BasePopup::handleEvent(event, mousePos); 

    // 2. Xu ly cac nut cua lop nay
    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    
    // 3. (MOI) Xu ly bang ban phim (Enter de Xac nhan, Escape de Huy)
    // *** SFML 3.0 API CHANGE ***
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            confirmButton.click(); // Gia lap click Xac Nhan
        } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
            hide(); // Dong popup
        }
    }
}

void ConfirmDeletePopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    
    // 1. Cap nhat nut [X] (tu lop cha)
    BasePopup::update(mousePos);
    
    // 2. Cap nhat cac nut cua lop nay
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
}
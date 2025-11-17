// GUI/Popups/MonTapFormPopup.cpp
#include "MonTapFormPopup.h"
#include "MonTapService.h" // Goi Service
#include "Validator.h"         // Goi Validator
#include "IDGenerator.h"

MonTapFormPopup::MonTapFormPopup(App& app)
    : BasePopup(app, "Them Mon Tap"), // Tieu de mac dinh
      currentMonTap(nullptr),
      errorMessage(font, "", 14)
{
    // Chinh lai kich thuoc panel cho form nho
    sf::Vector2f panelSize(450, 280);
    popupPanel.setSize(panelSize);
    popupPanel.setPosition(sf::Vector2f(
        (app.getWindow().getSize().x - panelSize.x) / 2.0f,
        (app.getWindow().getSize().y - panelSize.y) / 2.0f
    ));
    // Dat lai vi tri title/nut [X]
    title.setPosition(sf::Vector2f(popupPanel.getPosition().x + 20, popupPanel.getPosition().y + 15));
    closeButton.setPosition(popupPanel.getPosition().x + panelSize.x - 40, popupPanel.getPosition().y + 15);
    
    float pX = popupPanel.getPosition().x + 30;
    float pY = popupPanel.getPosition().y + 100; // Y cho input

    // Ten Mon Input
    tenMonInput.setup("Nhap ten mon tap", font, false);
    tenMonInput.setSize(390, 40);
    tenMonInput.setPosition(pX, pY);
    
    // Error Message
    // errorMessage.setFont(font);
    // errorMessage.setCharacterSize(14);
    errorMessage.setFillColor(Config::Danger);
    errorMessage.setPosition(sf::Vector2f(pX, pY + 50));

    // Buttons
    float btnY = popupPanel.getPosition().y + panelSize.y - 70;
    confirmButton.setup("Xac Nhan", font);
    confirmButton.setSize(120, 40);
    confirmButton.setPosition(pX + 70, btnY);
    confirmButton.setOnClick([this]() { handleSubmit(); });

    cancelButton.setup("Huy", font);
    cancelButton.setSize(120, 40);
    cancelButton.setPosition(pX + 260, btnY);
    cancelButton.setOnClick([this]() { hide(); });
}

void MonTapFormPopup::show(MonTap* mt, std::function<void()> onSuccess) {
    onSuccessCallback = onSuccess;
    errorMessage.setString("");
    
    if (mt == nullptr) {
        // --- THEM MOI ---
        title.setString("Them Mon Tap Moi");
        currentMonTap = nullptr;
        tenMonInput.setString("");
    } else {
        // --- SUA ---
        title.setString("Cap Nhat Mon Tap");
        currentMonTap = mt;
        tenMonInput.setString(mt->getTenMon());
    }
    
    tenMonInput.setFocus(true); // Tu dong focus vao o input
    BasePopup::show();
}

bool MonTapFormPopup::validate(std::string& tenMon) {
    tenMon = tenMonInput.getString();
    
    // Goi Validator ban da cung cap
    std::string errorMsg = Validator::validateTen(tenMon);
    if (!errorMsg.empty()) {
        errorMessage.setString(errorMsg);
        return false;
    }
    
    errorMessage.setString("");
    return true;
}

void MonTapFormPopup::handleSubmit() {
    std::string tenMon;
    if (!validate(tenMon)) {
        return; // Dung lai neu validate loi
    }
    
    if (currentMonTap == nullptr) {
        // --- THEM MOI (Goi Service) ---
        MonTapService::themMonTap(tenMon);
    } else {
        // --- SUA (Goi Service) ---
        MonTapService::suaMonTap(currentMonTap->getID(), tenMon);
    }
    
    // Kiem tra loi tu Service (vi du: Trung ten)
    // (Hien tai Service cua ban dang tu xu ly loi, nen chung ta chi can dong)
    
    if (onSuccessCallback) {
        onSuccessCallback(); // Goi callback de load lai bang
    }
    hide();
}

void MonTapFormPopup::drawContent(sf::RenderTarget& target) {
    // Ve label
    sf::Text label(font, "Ten Mon Tap:", 16);
    label.setFillColor(Config::TextNormal);
    label.setPosition(sf::Vector2f(
        popupPanel.getPosition().x + 30, 
        popupPanel.getPosition().y + 70
    ));
    
    target.draw(label);
    tenMonInput.draw(target);
    target.draw(errorMessage);
    confirmButton.draw(target);
    cancelButton.draw(target);
}

void MonTapFormPopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::handleEvent(event, mousePos); // Nut [X]

    confirmButton.handleEvent(event, mousePos);
    cancelButton.handleEvent(event, mousePos);
    
    // Gui su kien (Text, Key) cho input box
    tenMonInput.handleEvent(event);
    
    // Xu ly Enter de submit
    // *** SFML 3.0 API CHANGE ***
    if (auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Enter) {
            handleSubmit();
        }
    }
}

void MonTapFormPopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;
    BasePopup::update(mousePos); // Nut [X]
    
    confirmButton.update(mousePos);
    cancelButton.update(mousePos);
    tenMonInput.update(sf::Time::Zero); // Update dau nhay
}
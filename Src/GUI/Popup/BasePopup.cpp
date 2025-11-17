// GUI/Popups/BasePopup.cpp
#include "BasePopup.h"

BasePopup::BasePopup(App& app, const std::string& titleStr)
    : app(app), 
      font(app.getGlobalFont()), 
      isVisible(false),
      title(font, "", 24) // SFML 3.0
{
    sf::Vector2u windowSize = app.getWindow().getSize();
    
    // 1. Lop phu mo (den, trong suot 60%)
    backgroundOverlay.setSize(sf::Vector2f(
        static_cast<float>(windowSize.x), 
        static_cast<float>(windowSize.y)
    ));
    backgroundOverlay.setFillColor(sf::Color(0, 0, 0, 150)); // 150/255 opacity
    backgroundOverlay.setPosition(sf::Vector2f(0, 0));

    // 2. Popup panel (mac dinh 500x400)
    sf::Vector2f panelSize(500, 400);
    popupPanel.setSize(panelSize);
    popupPanel.setFillColor(Config::CardDark);
    popupPanel.setOutlineColor(Config::CardLight);
    popupPanel.setOutlineThickness(2);
    // Tu dong dat panel o giua man hinh
    popupPanel.setPosition(sf::Vector2f(
        (windowSize.x - panelSize.x) / 2.0f,
        (windowSize.y - panelSize.y) / 2.0f
    ));

    // 3. Tieu de (Title)
    title.setString(sf::String::fromUtf8(titleStr.begin(), titleStr.end()));
    title.setFillColor(Config::AccentCyan);
    title.setStyle(sf::Text::Bold);
    title.setPosition(sf::Vector2f(
        popupPanel.getPosition().x + 20, 
        popupPanel.getPosition().y + 15
    ));

    // 4. Nut [X] de dong
    closeButton.setup("X", font);
    closeButton.setSize(25, 25);
    closeButton.setColors(Config::Danger, sf::Color(255, 100, 100)); // Mau do
    closeButton.setPosition(
        popupPanel.getPosition().x + panelSize.x - 40, 
        popupPanel.getPosition().y + 15
    );
    // Gan hanh dong: khi click -> goi ham hide()
    closeButton.setOnClick([this]() {
        this->hide();
    });
}

BasePopup::~BasePopup() {
    // Ham huy ao
}

void BasePopup::show() { 
    isVisible = true; 
}

void BasePopup::hide() { 
    isVisible = false; 
}

bool BasePopup::getIsVisible() const { 
    return isVisible; 
}

void BasePopup::handleEvent(sf::Event event, sf::Vector2i mousePos) {
    if (!isVisible) return; // Neu dang an, khong xu ly gi ca
    
    // Luon xu ly nut [X]
    closeButton.handleEvent(event, mousePos);
    
    // (Cac lop con se override ham nay de xu ly them)
}

void BasePopup::update(sf::Vector2i mousePos) {
    if (!isVisible) return;

    // Luon cap nhat nut [X]
    closeButton.update(mousePos);

    // (Cac lop con se override ham nay de xu ly them)
}

void BasePopup::draw(sf::RenderTarget& target) {
    if (!isVisible) return;
    
    // 1. Ve lop mo truoc
    target.draw(backgroundOverlay); 
    
    // 2. Ve panel popup
    target.draw(popupPanel);        
    
    // 3. Ve cac thanh phan co ban (Title, Nut X)
    target.draw(title);             
    closeButton.draw(target);       
    
    // 4. Goi ham ao de ve noi dung cua lop con
    drawContent(target); 
}